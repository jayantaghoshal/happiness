/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <stdio.h>
#include <sys/mount.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define LOG_TAG "execns"
#include <log/log.h>

#define NS_DIR "/dev/vendor/netns/"

using std::cout;
using std::endl;

class FileDescriptor {
  public:
    explicit FileDescriptor(int fd) : mFd(fd) {}
    FileDescriptor(const FileDescriptor&) = delete;
    ~FileDescriptor() {
        if (mFd != -1) {
            close(mFd);
            mFd = -1;
        }
    }
    int get() const { return mFd; }
    FileDescriptor& operator=(const FileDescriptor&) = delete;

  private:
    int mFd;
};

std::string readNamespacePid(const std::string& ns) {
    std::string pid_path(NS_DIR);
    std::ifstream ifs(pid_path + ns + ".pid");
    std::string ns_pid((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return ns_pid;
}

bool changeNetworkNamespace(const std::string& ns) {
    // There is a file in the net namespace dir with the name "<namespace>.pid".
    // This file contains the pid of the createns process that created the namespace.
    //
    // To switch network namespace we're going to call setns which requires an
    // open file descriptor to /proc/<pid>/ns/net where <pid> refers to a
    // process already running in that namespace. So using the pid from the file
    // above we can determine which path to use.
    std::string pid = readNamespacePid(ns);
    if (pid.empty()) {
        return false;
    }
    std::string ns_path("/proc/");
    ns_path.append(pid + "/ns/net");

    FileDescriptor fd{open(ns_path.c_str(), O_RDONLY | O_CLOEXEC)};
    if (fd.get() == -1) {
        cout << "Cannot open network namespace '" << ns << "' at '" << ns_path.c_str() << " : " << strerror(errno)
             << endl;
        return false;
    }

    if (setns(fd.get(), CLONE_NEWNET) == -1) {
        cout << "Cannot set network namespace '" << ns << " : " << strerror(errno) << endl;
        return false;
    }

    if (unshare(CLONE_NEWNS) < 0) {
        cout << "Unshare failed: " << strerror(errno) << endl;
        return false;
    }
    /* Mount a version of /sys that describes the network namespace */
    if (umount2("/sys", MNT_DETACH) < 0) {
        cout << "Umount of /sys failed: " << strerror(errno) << endl;
        return false;
    }
    if (mount("", "/sys", "sysfs", 0, NULL) < 0) {
        cout << "Mount of /sys failed: " << strerror(errno) << endl;
        return false;
    }
    return true;
}

/**
 * Execute a given command with num_args number of parameters that are located
 * in args. The first parameter in args is the command that should be run
 * followed by its arguments.
 */
int execCommand(int num_args, char** args) {
    if (num_args <= 0 || args == nullptr || args[0] == nullptr) {
        cout << "No command specified" << endl;
        return 1;
    }

    std::vector<char*> arguments;
    // Place all the arguments in the vector and the terminating null
    arguments.insert(arguments.begin(), args, args + num_args);
    arguments.push_back(nullptr);

    if (execvp(args[0], arguments.data()) == -1) {
        // Save errno in case it gets changed by printing stuff.
        int error = errno;
        cout << "Could not execute command: " << strerror(error) << endl;
        return error;
    }
    // execvp never returns unless it fails so this is just to return something.
    return 0;
}

/**
 * Enter a given network namespace argv[1] and execute command argv[2] with
 * options argv[3..argc-1] in that namespace.
 */
int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << argv[0] << " <namespace> <program> [options...]" << endl;
        return 1;
    }

    if (!changeNetworkNamespace(argv[1])) {
        return 1;
    }

    return execCommand(argc - 2, &argv[2]);
}
