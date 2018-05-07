# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import gdb
from gdb.FrameDecorator import FrameDecorator

#USAGE:
# In gdb console or .gdbinit:
#   source MY/PATH/TO/vendor/volvocars/gdb_decorator.py


class MyFrameDecorator(FrameDecorator):
    def __init__(self, fobj):
        super(self.__class__, self).__init__(fobj)
        self.fobj = fobj

    def function(self):
        frame = self.fobj.inferior_frame()
        name = str(frame.name())
        name = name.replace("android::hardware::automotive::vehicle::V2_0::impl::", "vhalimpl::")
        name = name.replace("android::hardware::automotive::vehicle::V2_0::", "vhal20::")
        name = name.replace("vendor::volvocars::hardware::settings::V1_0::implementation::", "vccprofimpl::")
        name = name.replace("vendor::volvocars::hardware::settings::V1_0::profiles::", "vccprofiles::")
        name = name.replace("vendor::volvocars::hardware::settings::V1_0::", "vccsettings::")
        name = name.replace("android::hardware::", "ahw::")
        name = name.replace("std::__1::", "std::")
        name = name.replace("std::shared_ptr", "shared_ptr")
        name = name.replace("std::unique_ptr", "unique_ptr")
        name = name.replace("std::function", "function")

        orig_filename = super(self.__class__, self).filename()
        if orig_filename is not None and "libcxx" in orig_filename:
            return "____STDLIB__" + name
        return name

    def filename(self):
        fname = super(self.__class__, self).filename()
        #fname = fname.replace("vendor/volvocars/hardware/", "vcchw/")
        if fname is not None:
            fname = fname.split("/")[-1]
        return fname

    def address(self):
        # Hide the stack address
        return None


class MyFrameFilter:
    def __init__(self):
        self.name = "my_filter"
        self.priority = 100
        self.enabled = True
        gdb.frame_filters[self.name] = self     # Register self to the global frame_filter dict

    def filter(self, frame_iter):
        if frame_iter is None:
            return None
        return (MyFrameDecorator(f) for f in frame_iter)

MyFrameFilter()