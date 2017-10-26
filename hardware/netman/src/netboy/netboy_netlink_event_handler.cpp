/*
 * ===========================================================================
 * [2017]- Volvo Car Corporation
 * All Rights Reserved
 * LEGAL
 * NOTICE:  All information (including intellectual and technical concepts) contained herein is, and remains the
 * property of or licensed to Volvo Car Corporation. This information is proprietary to Volvo Car Corporation and may be
 * covered by patents or patent applications. This information is protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is strictly forbidden unless prior written
 * permission is obtained from Volvo Car Corporation.
 */

#include <cutils/log.h>
#include <ftw.h>
#include <limits.h>
#include <linux/ethtool.h>
#include <linux/rtnetlink.h>
#include <linux/socket.h>
#include <net/if.h>
#include <unistd.h>
#include <fstream>
#include "netutils.h"

#include "netboy_netlink_event_handler.h"
#include "rule_handler.h"

#define LOG_TAG "Netboyd"

namespace {

const char *const ATTR_SUBSYSTEM = "SUBSYSTEM";
const char *const ATTR_DEVTYPE = "DEVTYPE";
const char *const ATTR_DEVPATH = "DEVPATH";
const char *const ATTR_DRIVER = "DRIVER";
const char *const ATTR_INTERFACE = "INTERFACE";
const char *const ATTR_IFINDEX = "IFINDEX";
const char *const ATTR_ACTION = "ACTION";

struct NetDeviceAttr {
  std::string interface_name;  // name of interface e.g. eth0, wlan1 etc
  std::string subsystem;       // subsystem e.g. usb, pci, net
  std::string driver;          // driver used for device
  std::string devpath;         // device path in /sys/devices
  std::string devtype;
  std::string action;  // add, remove, change etc.
};

std::string ExtractAttribute(const std::string &cursor_line, const std::string &attribute) {
  std::string value;
  if (!cursor_line.empty()) {
    value = std::string(cursor_line.c_str() + attribute.length() + 1);
  }
  return value;
}

void ReadDeviceAttr(const std::string &line, NetDeviceAttr &child_device) {
  // TODO: Refactor function to make it compact some later time

  // Only read attributes if there values are not already set. As attributes for child are read before parent
  // attributes; this will result in looking at attribute which is more relevant.

  if (child_device.interface_name.empty() && line.find(ATTR_INTERFACE) != std::string::npos) {
    child_device.interface_name = ExtractAttribute(line, ATTR_INTERFACE);
    return;
  }

  if (child_device.devpath.empty() && line.find(ATTR_DEVPATH) != std::string::npos) {
    child_device.devpath = ExtractAttribute(line, ATTR_DEVPATH);
    return;
  }

  if (child_device.action.empty() && line.find(ATTR_ACTION) != std::string::npos) {
    child_device.action = ExtractAttribute(line, ATTR_ACTION);
    return;
  }

  if (child_device.subsystem.empty() && line.find(ATTR_SUBSYSTEM) != std::string::npos) {
    child_device.subsystem = ExtractAttribute(line, ATTR_SUBSYSTEM);
    return;
  }

  if (child_device.devtype.empty() && line.find(ATTR_DEVTYPE) != std::string::npos) {
    child_device.devtype = ExtractAttribute(line, ATTR_DEVTYPE);
    return;
  }

  if (child_device.driver.empty() && line.find(ATTR_DRIVER) != std::string::npos) {
    child_device.driver = ExtractAttribute(line, ATTR_DRIVER);
    return;
  }

  if (child_device.devtype.empty() && line.find(ATTR_DEVTYPE) != std::string::npos) {
    child_device.devtype = ExtractAttribute(line, ATTR_DEVTYPE);
    return;
  }
}

void ReadParentDeviceAttr(NetDeviceAttr &child_device) {
  // Adjust devpath to access attributes for parent node
  std::fstream parent_attr(std::string("/sys/") + child_device.devpath + "/../../uevent");
  if (parent_attr.is_open()) {
    std::string line;
    while (parent_attr >> line) {
      ReadDeviceAttr(line, child_device);
    }
  }
}

void RuleMatchAndRun(const NetDeviceAttr &device) {
  using namespace vcc::netman;
  // Create and fill structure for rule matching
  NetboyRule rule{device.devpath, device.subsystem, device.devtype, device.interface_name, device.driver, nullptr};
  std::shared_ptr<RuleAction> action;

  if (RuleHandler::getInstance().getMatchingRule(rule, action)) {
    if (!action) {
      ALOGE("Found a matching rule but ACTION is NULL");
      return;
    }

    if (!(action->intent_ == Intent::MOVE || action->intent_ == Intent::MOVE_AND_RENAME ||
          action->intent_ == Intent::RENAME))
      return;

    TakeInterfaceDown(device.interface_name.c_str());

    if (action->intent_ == Intent::MOVE) {
      std::string ns = std::static_pointer_cast<RuleActionMove>(action)->NEW_NS;

      MoveNetworkInterfaceToNamespace(device.interface_name, ns);
      BringInterfaceUp(device.interface_name.c_str(), ns);

    } else if (action->intent_ == Intent::RENAME) {
      std::string name = std::static_pointer_cast<RuleActionRename>(action)->NEW_NAME;

      MoveNetworkInterfaceToNamespace(device.interface_name, "", name);
      BringInterfaceUp(name.c_str());

    } else if (action->intent_ == Intent::MOVE_AND_RENAME) {
      std::string ns = std::static_pointer_cast<RuleActionMoveAndRename>(action)->NEW_NS;
      std::string name = std::static_pointer_cast<RuleActionMoveAndRename>(action)->NEW_NAME;

      MoveNetworkInterfaceToNamespace(device.interface_name, ns, name);
      BringInterfaceUp(name, ns);
    }
  }
}

}  // namespace

namespace vcc {
namespace netman {

void NetboyNetlinkEventHandler::HandleEvent(NetlinkEventData *eventData) {
  switch (eventData->eventType) {
    case NetlinkEventType::NETLINK_UEVENT: {
      NetlinkUevent *pData = static_cast<NetlinkUevent *>(eventData);
      if (pData) HandleUevent(pData->uevent, pData->length);
    } break;
    default:
      break;
  }
}

int NetboyNetlinkEventHandler::HandleSysfsEntry(const char *filepath, const struct stat *info, const int typeflag,
                                                struct FTW *pathinfo) {
  // NOTE: NTFW requires callback function to return 0 as an indication proceed

  // As per SysFs standard, entries in '/sys/class/net' are symlinks to entries in '/sys/devices'
  if (typeflag != FTW_SL) {
    return 0;
  }

  // Each networking device in /sys/class/net/xxx contains uevent file describing attributes of interface
  std::fstream file(std::string(filepath) + "/uevent", std::ios::in);
  if (!file.is_open()) {
    ALOGE("Failed to open file %s", (std::string(filepath) + "/uevent").c_str());
    return 0;
  }

  std::string segment;
  NetDeviceAttr device_attr;
  while (file >> segment) {
    ReadDeviceAttr(segment, device_attr);
  }

  const std::vector<std::string> ignored_interface_list{"lo", "sit0"};
  auto it = std::find_if(ignored_interface_list.cbegin(), ignored_interface_list.cend(),
                         [&device_attr](const std::string &item) { return (device_attr.interface_name == item); });
  if (it != ignored_interface_list.cend()) {
    return 0;  // filter out 'lo' and 'sit0'
  }

  char path[PATH_MAX];
  auto len = readlink(filepath, path, sizeof(path) - 1);
  if (len == -1) {
    ALOGE("Failed to resolve symlink for %s", path);
    return 0;  // readlink returns negative value in case of error
  }
  path[len] = '\0';  // readlink doesn't bother to add null character, so let's do it

  // +6 to truncate '../../' in path so it will be relative to '/sys/'
  device_attr.devpath = std::string(path + 6);

  ReadParentDeviceAttr(device_attr);
  RuleMatchAndRun(device_attr);
  return 0;
}

int NetboyNetlinkEventHandler::SysfsNetSubsystemWalker() {
  const int openFds = 20;  // resonable number I suppose

  // Do not follow symlinks
  return nftw("/sys/class/net/", HandleSysfsEntry, openFds, FTW_PHYS);
}

// handle new message
void NetboyNetlinkEventHandler::HandleUevent(const char *uevent, const int message_length) {
  if (!uevent) return;

  const char *const ADD_ACTION = "add@";

  // Currently only add@ event are handled. Can be extended as per need
  if (message_length < static_cast<int>(std::string(ADD_ACTION).length()) ||
      std::string(uevent).find(ADD_ACTION) == std::string::npos)
    return;

  // advance buffer past first segment
  int index = strlen(uevent) + 1;
  bool read_parent = false;

  NetDeviceAttr device;
  while (index < message_length) {
    std::string line(uevent + index);
    // see if it is a networking event
    if (!read_parent && (line.find(ATTR_IFINDEX) != std::string::npos)) {
      read_parent = true;
    }

    ReadDeviceAttr(line, device);
    index += line.length() + 1;
  }

  if (read_parent) {
    ReadParentDeviceAttr(device);
    RuleMatchAndRun(device);
  }
}

}  // namespace netman
}  // namespace vcc
