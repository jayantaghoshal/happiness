/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <cstdlib>
#include <sstream>
#include <string>

#define LOG_TAG "dnsmasq_wrapper"
#include <cutils/log.h>

int main() {
    ALOGD("dnsmasq_wrapper is starting ...");

    const std::string range = "10.0.0.2,10.0.0.2,static";
    const std::string host = "06:00:00:00:00:06,10.0.0.2";
    const std::string interface = "rtp_aud0";

    /*
     * http://www.thekelleys.org.uk/dnsmasq/docs/dnsmasq-man.html
     *
     * -k          --> Keep in foreground
     * --no-resolv --> Don't read /etc/resolv.conf. Get upstream servers only from the command line or the dnsmasq
     * configuration file.
     * --no-hosts  --> Don't read the hostnames in /etc/hosts.
     * --pid-file  --> Specify an alternate path for dnsmasq to record its process-id in. Normally /var/run/dnsmasq.pid.
     *                 ... missing parameters are allowed and switch off functions, for instance "--pid-file" disables
     *                 writing a PID file...
     *
     * We need to disable the use of pid file because it tries to write on a read-only partition
     */

    const std::string cmd = "/vendor/bin/dnsmasq --dhcp-range " + range + " --dhcp-host " + host + " --interface " +
                            interface + " -k --no-resolv --no-hosts --pid-file <&-";

    int command_status = std::system(cmd.c_str());

    if ((command_status < 0) || !WIFEXITED(command_status) || WEXITSTATUS(command_status) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    ALOGE("dnsmasq_wrapper is exiting ...");
}
