# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

#! /usr/bin/env python
# -*- coding: utf-8 -*-
import re
import sys
import os

from com.dtmilano.android.viewclient import ViewClient
from vcc_utilities import *

TAG = 'CULEBRA'

# Use delay to allow animations to happen before dumping views
_s = 1
_v = '--verbose' in sys.argv


kwargs1 = {'ignoreversioncheck': False, 'verbose': False, 'ignoresecuredevice': False}
device, serialno = ViewClient.connectToDeviceOrExit(**kwargs1)
kwargs2 = {'forceviewserveruse': False, 'useuiautomatorhelper': False, 'ignoreuiautomatorkilled': True, 'autodump': False, 'startviewserver': True, 'compresseddump': True}
vc = ViewClient(device, serialno, **kwargs2)

pressHomeButton()

# Start by dumping all views and verify that they are all present
# Skip looking for views containing text in general, since text can change over time
vc.dump(window=-1)

no_id1 = vc.findViewByIdOrRaise("id/no_id/1")
com_volvocars_launcher___id_pager = vc.findViewByIdOrRaise("com.volvocars.launcher:id/pager")
com_volvocars_launcher___id_recycler_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/recycler_view")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_listview_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/listview_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_temperature_slider_pane_left = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_left")
com_volvocars_launcher___id_status_bar_clock = vc.findViewByIdOrRaise("com.volvocars.launcher:id/status_bar_clock")
com_volvocars_launcher___id_temperature_l_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_l_text")
com_volvocars_launcher___id_seat_l = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_l")
com_volvocars_launcher___id_seat_r = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_r")
com_volvocars_launcher___id_temperature_r_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_r_text")
com_volvocars_launcher___id_temperature_slider_pane_right = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_right")

# Show the temp slider by clicking temp and then dump window again
com_volvocars_launcher___id_temperature_l_text.touch()
vc.dump(window=-1)

# Make sure all expected views can be found
no_id1 = vc.findViewByIdOrRaise("id/no_id/1")
com_volvocars_launcher___id_pager = vc.findViewByIdOrRaise("com.volvocars.launcher:id/pager")
com_volvocars_launcher___id_recycler_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/recycler_view")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_listview_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/listview_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_status_bar_clock = vc.findViewByIdOrRaise("com.volvocars.launcher:id/status_bar_clock")
com_volvocars_launcher___id_temperature_l_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_l_text")
com_volvocars_launcher___id_seat_l = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_l")
com_volvocars_launcher___id_seat_r = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_r")
com_volvocars_launcher___id_temperature_r_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_r_text")
com_volvocars_launcher___id_temperature_slider_pane_right = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_right")

# Temp adjust id is used to know where to start dragging, this is the id of the slider
left_slider = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_adjust")

# Make sure we reset the slider all the way down to the bottom so we have a starting point
vc.swipe(left_slider.getX(), left_slider.getY(), left_slider.getX(), left_slider.getY()+800, 500)
vc.sleep(_s)
vc.dump(window=-1)

# Move slider half way up, fetch it again to make sure it is still available after dump
left_slider = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_adjust")
vc.swipe(left_slider.getX(), left_slider.getY(), left_slider.getX(), left_slider.getY()-300, 2000)

# Close the temp slider and refresh view
close_button = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_close_highlight")
close_button.touch()
vc.sleep(_s)
vc.dump(window=-1)

# Check temperature on left is set to 21
current_temp = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_l_text")
if current_temp.getText() != '21':
    raise ValueError('Temperature was expected to be 21')
