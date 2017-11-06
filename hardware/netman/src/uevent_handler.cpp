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

#include "uevent_handler.h"
#include <cutils/log.h>
#include <fstream>
#include <string>

#define LOG_TAG "Netmand"

namespace vcc {
namespace netman {

int UeventHandler::SysfsNetSubsystemWalker() {
    const int openFds = 20;  // resonable number I suppose

    // Do not follow symlinks
    return nftw("/sys/class/net/", HandleSysfsEntry, openFds, FTW_PHYS);
}

void UeventHandler::ReadDeviceAttr(const std::string &line, NetDeviceAttr &child_device) {
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

void UeventHandler::ReadParentDeviceAttr(NetDeviceAttr &child_device) {
    // Adjust devpath to access attributes for parent node
    std::fstream parent_attr(std::string("/sys/") + child_device.devpath + "/../../uevent");
    if (parent_attr.is_open()) {
        std::string line;
        while (parent_attr >> line) {
            ReadDeviceAttr(line, child_device);
        }
    }
}

std::string UeventHandler::ExtractAttribute(const std::string &cursor_line, const std::string &attribute) {
    std::string value;
    if (!cursor_line.empty()) {
        value = std::string(cursor_line.c_str() + attribute.length() + 1);
    }
    return value;
}

int UeventHandler::HandleSysfsEntry(const char *filepath, const struct stat *info, const int typeflag,
                                    struct FTW *pathinfo) {
    // NOTE: NTFW requires callback function to return 0 as an indication proceed

    // As per SysFs standard, entries in '/sys/class/net' are symlinks to entries in '/sys/devices'
    if (typeflag != FTW_SL) {
        return 0;
    }

    // Each networking device in /sys/class/net/xxx contains uevent file describing attributes of interface
    std::fstream file(std::string(filepath) + "/uevent");
    if (!file.is_open()) {
        ALOGW("Failed to open file %s", (std::string(filepath) + "/uevent").c_str());
        return 0;
    }

    const std::string ATTR_INTERFACE = "INTERFACE";
    std::string segment;
    std::string interface_name;
    while (file >> segment) {
        if (segment.find(ATTR_INTERFACE) != std::string::npos) {
            interface_name = std::string(segment.c_str() + ATTR_INTERFACE.length() + 1);
            break;
        }
    }

    const std::vector<std::string> ignored_interface_list{"lo", "sit0"};
    auto it = std::find_if(ignored_interface_list.cbegin(), ignored_interface_list.cend(),
                           [&interface_name](const std::string &item) { return (interface_name == item); });
    if (it != ignored_interface_list.cend()) {
        return 0;  // filter out 'lo' and 'sit0'
    }

    // Ask kernel to regenerate event by writing "add" to uevent file. new event will be handled in main thread.
    file << "add";
    if (!file) {
        ALOGW("kernel will not resend KOBJECT_UEVENT for %s", interface_name.c_str());
    }

    return 0;
}

}  // namespace netman
}  // namespace vcc
