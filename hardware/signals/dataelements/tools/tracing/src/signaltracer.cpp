/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <systemd/sd-bus.h>
#include <systemd/sd-event.h>
#include <map>
#include <vector>

static const std::string DE_SIGNAL_CHANGED_NAME = "DESignalChanged";
enum Dir { IN, OUT, INTERNAL };

struct DBUSDescr {
    std::string interface;
    std::string objectpath;
};
static std::map<Dir, DBUSDescr> interfaces{
        {IN, {"com.ihu.VehicleSignalsManager.In", "/com/ihu/VehicleSignalsManager/In"}},
        {OUT, {"com.ihu.VehicleSignalsManager.Out", "/com/ihu/VehicleSignalsManager/Out"}},
        {INTERNAL, {"com.ihu.VehicleSignalsManager.Internal", "/com/ihu/VehicleSignalsManager/Internal"}}};

static std::vector<std::string> matchStrings;
static int printType = 0xff;
static bool listSignals = false;

static sd_bus* bus_ = NULL;
sd_event* event = NULL;
sd_event_source* event_source = NULL;

bool shouldBePrinted(const std::string& signal, Dir dir) {
    if ((printType == 0xff && matchStrings.empty()) || (printType == dir && matchStrings.empty())) {
        return true;
    } else if (printType != 0xff && (printType != dir))  // Not all signals printed check if correct type
    {
        return false;
    }

    std::string searchStr;

    bool foundFirst = false, foundLast = false;
    for (auto matchStr : matchStrings) {
        std::size_t idxfoundFirst = matchStr.find("*");
        std::size_t idxfoundLast = matchStr.rfind("*");
        if (idxfoundFirst != std::string::npos) {
            foundFirst = (idxfoundFirst == 0);
            foundLast = (idxfoundLast == (matchStr.size() - 1));

            if (foundFirst && foundLast && idxfoundLast > idxfoundFirst)  // E.g match string is *Prof*
            {
                searchStr = matchStr.erase(matchStr.size() - 1, 1);
                searchStr = searchStr.erase(0, 1);

                if (signal.find(searchStr) != std::string::npos) return true;
            } else if (foundFirst)  // E.g. *Sts Search pattern last in string
            {
                // return s.rfind(suffix) == (s.size()-suffix.size());
                searchStr = matchStr.erase(0, 1);
                if ((signal.rfind(searchStr) == signal.size() - searchStr.size())) return true;
            } else if (foundLast) {
                searchStr = matchStr.erase(matchStr.size() - 1, 1);
                if (signal.find(searchStr) == 0) return true;
            }
        } else  // Exact match
        {
            if (matchStr == signal) return true;
        }
    }
    return false;
}

void printSignal(const std::string& signal, const std::string& signalData, Dir direction) {
    std::string dirStr;

    if (shouldBePrinted(signal, direction)) {
        switch (direction) {
            case IN:
                dirStr = "<-";
                break;
            case OUT:
                dirStr = "->";
                break;
            case INTERNAL:
                dirStr = "[internal]";
                break;
        }

        printf("%s  %s\t%s\n", dirStr.c_str(), signal.c_str(), signalData.c_str());
    }
}

bool readSignalData(const std::string& signalName, Dir direction, std::string& signalDataOut) noexcept {
    char* signalValue = NULL;
    sd_bus_error error = SD_BUS_ERROR_NULL;

    auto retval = sd_bus_get_property_string(
            bus_, interfaces[direction].interface.c_str(), interfaces[direction].objectpath.c_str(),
            interfaces[direction].interface.c_str(), signalName.c_str(), &error, &signalValue);

    if (retval < 0) {
        return false;
    }

    signalDataOut = signalValue;

    free(signalValue);

    return true;
}

int signals_changed(sd_bus_message* message, void* userdata, sd_bus_error* error) {
    const char* s;
    int result;

    result = sd_bus_message_read(message, "s", &s);

    if (result < 0) {
        printf("sd_bus_message_read() %d  %s \n", result, strerror(-result));
        return result;
    }

    Dir d = static_cast<Dir>((intptr_t)userdata);

    std::string signalDataOut;
    readSignalData(s, static_cast<Dir>((intptr_t)userdata), signalDataOut);
    printSignal(s, signalDataOut, d);
}

void opensdbus(int argc) {
    int ret_val = sd_bus_open_system(&bus_);
    if (ret_val < 0) {
        printf("Failed to open system D-Dus.\n");
        return;
    }
}

void addMatchForChangedSignals() {
    std::string matchString;

    for (auto interface : interfaces) {
        matchString = "type='signal',sender='";
        matchString += interface.second.interface + "',";
        matchString += "interface='" + interface.second.interface + "',";
        matchString += "member='" + DE_SIGNAL_CHANGED_NAME + "'";

        int ret_val =
                sd_bus_add_match(bus_, NULL, matchString.c_str(), signals_changed, (void*)(intptr_t)interface.first);

        if (ret_val < 0) {
            printf("Failed to add match for SignalsChanged: ret_val= %d matchstring = %s \n", ret_val,
                   matchString.c_str());
        }
    }
}

void closesdbus() {
    sd_bus_close(bus_);

    bus_ = NULL;
}

void printCurrentSignalsForDir(Dir dir) {
    sd_bus_message* reply = NULL;
    sd_bus_error error = SD_BUS_ERROR_NULL;

    // Read all property names for the given interface
    int r = sd_bus_call_method(bus_, interfaces[dir].interface.c_str(), interfaces[dir].objectpath.c_str(),
                               "org.freedesktop.DBus.Properties", "GetAll", &error, &reply, "s",
                               interfaces[dir].interface.c_str());
    if (r < 0) {
        printf("Failed to sd_bus_call_method org.freedesktop.DBus.Properties.GetAll: ret_val= %d \n", r);
        return;
    }

    r = sd_bus_message_enter_container(reply, 'a', "{sv}");
    if (r < 0) {
        printf("Failed to sd_bus_message_enter_container 1 org.freedesktop.DBus.Properties.GetAll: ret_val= %d \n", r);
        return;
    }

    for (;;) {
        size_t sz = 0;
        const char* name;

        r = sd_bus_message_enter_container(reply, 'e', "sv");
        if (r < 0) {
            printf("Failed to sd_bus_message_enter_container  2 org.freedesktop.DBus.Properties.GetAll: ret_val= %d \n",
                   r);
            return;
        }

        if (r == 0) break;

        r = sd_bus_message_read(reply, "s", &name);  // Read the signal name
        if (r < 0) {
            printf("Failed to read signal name ret_val= %d \n", r);
            return;
        } else {
            std::string signalDataOut;
            readSignalData(name, dir, signalDataOut);
            if (!signalDataOut.empty()) printSignal(name, signalDataOut, dir);
        }

        r = sd_bus_message_skip(reply, "v");
        if (r < 0) {
            printf("Failed to sd_bus_message_skip ret_val= %d \n", r);
            return;
        }

        r = sd_bus_message_exit_container(reply);
        if (r < 0) {
            printf("Failed to sd_bus_message_exit_container 2 ret_val= %d \n", r);
            return;
        }
    }

    r = sd_bus_message_exit_container(reply);
    if (r < 0) {
        printf("Failed to sd_bus_message_exit_container 1 ret_val= %d \n", r);
        return;
    }
}

void printCurrentSignals() {
    switch (printType) {
        case IN:
        case OUT:
        case INTERNAL:
            printCurrentSignalsForDir(static_cast<Dir>(printType));
            break;
        default:  // Print all interfaces
            printCurrentSignalsForDir(IN);
            printCurrentSignalsForDir(OUT);
            printCurrentSignalsForDir(INTERNAL);
    }
}

void printHelp() {
    printf("Usage: signaltrace [in|out|internal] [SIGNALS] [-l, -list]\n");
    printf("Example: 'signaltrace' trace all signals\n");
    printf("Example: 'signaltrace in' trace all signals received by IHU\n");
    printf("Example: 'signaltrace Prof*' trace all signals that starts with \"Prof\"\n");
    printf("Example: 'signaltrace in *Mod*' trace all in signals that contains string \"Mod\"\n");
    printf("Example: 'signaltrace in -list' First lists all received in signals last received value then trace all in "
           "signals\n");
    printf("Example: 'signaltrace *Prof* -l' First lists all(in/out/internal) signals last received value then trace "
           "all(in/out/internal)signals with "
           "wildcard \"*Prof*\"\n");
}
bool handleArguments(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "in") {
            printType = IN;
        } else if (arg == "out") {
            printType = OUT;
        } else if (arg == "internal") {
            printType = INTERNAL;
        } else if (arg == "--help" || arg == "-help") {
            printHelp();
            return false;
        } else if (arg == "-l" || arg == "--list" || arg == "-list") {
            listSignals = true;
        } else  // All signals traced, add the first argument as a signal to match
        {
            matchStrings.push_back(argv[i]);
        }
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (!handleArguments(argc, argv)) return 0;

    opensdbus(argc);
    addMatchForChangedSignals();

    if (listSignals) printCurrentSignals();

    // Set up sd-event for message loop

    int r = sd_event_default(&event);
    if (r < 0) {
        printf("Failed creating event: %s\n", strerror(-r));
        return r;
    }
    r = sd_bus_attach_event(bus_, event, 0);
    if (r < 0) {
        printf("Failed attaching event: %s\n", strerror(-r));
        return r;
    }
    // Start the message loop, blocks forever
    r = sd_event_loop(event);
    if (r < 0) {
        printf("Failed event loop: %s \n", strerror(-r));
        // cleanup();
        return r;
    }

    closesdbus();
}
