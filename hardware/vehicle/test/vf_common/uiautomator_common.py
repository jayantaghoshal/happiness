# Copyright 2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

import logging
import time
from vts.runners.host import asserts
from uiautomator import device as device_default

class AppId:
    app_context_vehiclefunctions = "com.volvocars.vehiclefunctions:id/"
    app_context_halmodulesink = "com.volvocars.halmodulesink:id/"
    vehiclefunctions_view = "com.volvocars.vehiclefunctions:id/recyclerView"

class ButtonId:
    fan_off = "com.volvocars.launcher:id/fan_off"
    fan_level_1 = "com.volvocars.launcher:id/fan_one"
    fan_level_2 = "com.volvocars.launcher:id/fan_two"
    fan_level_3 = "com.volvocars.launcher:id/fan_three"
    fan_level_4 = "com.volvocars.launcher:id/fan_four"
    fan_level_5 = "com.volvocars.launcher:id/fan_five"
    fan_max = "com.volvocars.launcher:id/fan_max"
    temperature_controller_left = "com.volvocars.launcher:id/temperature_controller_driver"
    temperature_controller_right = "com.volvocars.launcher:id/temperature_controller_passenger"
    temperature_plus_button = "com.volvocars.launcher:id/plus_button"
    temperature_minus_button = "com.volvocars.launcher:id/minus_button"

class UIAutomatorCommon:
    def __init__(self):
        self.device = device_default

    def waitUntilViewAvailable(self, view_name, timeout_seconds=15):
        logging.info("Waiting until view " + view_name + " is available")
        start_time = time.time()
        while time.time() - start_time <= timeout_seconds:
            view = self.device(resourceId=view_name)
            if view.exists:
                logging.info("View is found: " + view_name)
                return
            else:
                logging.info("Waiting...%s", str(time.time() - start_time))
            time.sleep(0.2)
        asserts.assertTrue(False, "Time out! View is still not visible: " + view_name)

    def getViewOrRaise(self, viewId):
        view = self.device(resourceId=viewId)
        if view.exists:
            return view
        raise Exception("View with id %r not found", viewId)

    def scrollDownAndFindViewByIdOrRaise(self, viewId, maxFlings=15):
        # TODO: Check if end of scrollable area instead of maxflings
        view = self.scrollDownAndFindViewById(viewId, maxFlings)
        asserts.assertNotEqual(None, view, "Did not find view")
        return view

    def scrollDownAndFindViewById(self, viewId, maxFlings=15):
        print("Searching for view with id -> ") + str(viewId)
        # TODO: Check if end of scrollable area instead of maxflings
        for n in range(maxFlings):
            view = self.device(resourceId=viewId)
            if view.exists:
                return view
            self.device.drag(100, 300, 100, 100, 10)
        return None

    def scrollAndAssertViewsAreNotFound(self, ids, maxFlings=10):
        # TODO: Check if end of scrollable area instead of maxflings
        for n in range(maxFlings):
            for id in ids:
                view = self.device(resourceId=id)
                asserts.assertFalse(view.exists, "Found view with id=%s when no view was expected" % id)
            self.device.drag(100, 300, 100, 100, 10)
        self.device(resourceId=AppId.vehiclefunctions_view).scroll.toBeginning()   # Make sure that it starts the search from the top
