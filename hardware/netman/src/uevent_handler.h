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

#ifndef _UEVENT_HANDLER_H_
#define _UEVENT_HANDLER_H_

#include <ftw.h>
#include <string>
#include "interface_configuration.h"
#include "uevent_listener.h"

namespace vcc {
namespace netman {

class UeventHandler {
  public:
    struct NetDeviceAttr {
        std::string interface_name;  // name of interface e.g. eth0, wlan1 etc
        std::string subsystem;       // subsystem e.g. usb, pci, net
        std::string driver;          // driver used for device
        std::string devpath;         // device path in /sys/devices
        std::string devtype;
        std::string action;  // add, remove, change etc.
    };

    const char *const ATTR_SUBSYSTEM = "SUBSYSTEM";
    const char *const ATTR_DEVTYPE = "DEVTYPE";
    const char *const ATTR_DEVPATH = "DEVPATH";
    const char *const ATTR_DRIVER = "DRIVER";
    const char *const ATTR_INTERFACE = "INTERFACE";
    const char *const ATTR_IFINDEX = "IFINDEX";
    const char *const ATTR_ACTION = "ACTION";

    UeventHandler() = default;
    virtual ~UeventHandler() = default;

    UeventHandler(const UeventHandler &) = delete;
    UeventHandler &operator=(const UeventHandler &) = delete;

    UeventHandler(const UeventHandler &&) = delete;
    UeventHandler &operator=(const UeventHandler &&) = delete;

    static int SysfsNetSubsystemWalker();
    virtual void HandleEvent(const char *uevent, int message_length) = 0;

  protected:
    void ReadParentDeviceAttr(NetDeviceAttr &child_device);
    void ReadDeviceAttr(const std::string &line, NetDeviceAttr &child_device);
    std::string ExtractAttribute(const std::string &cursor_line, const std::string &attribute);

  private:
    static int HandleSysfsEntry(const char *filepath, const struct stat * /*info*/, int typeflag,
                                struct FTW * /*pathinfo*/);
};

}  // namespace netman
}  // namespace vcc

#endif  // _UEVENT_HANDLER_H_
