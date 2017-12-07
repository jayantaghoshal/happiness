# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

#! /usr/bin/env python
# -*- coding: utf-8 -*-
import re
import sys
import os
import time

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

# Press home button first
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

# Swipe full width to the right to get into vechicle functions pane
vc.swipe(no_id1.getX(), no_id1.getY(), no_id1.getX()+no_id1.getWidth(), no_id1.getY(), 2000)

vc.dump(window=-1)
no_id1 = vc.findViewByIdOrRaise("id/no_id/1")
hold_button = vc.findViewWithTextOrRaise(u'Hold')
charge_button = vc.findViewWithTextOrRaise(u'Charge')
ext_sound_button = vc.findViewWithTextOrRaise(u'Exterior Sound')
headrest_button = vc.findViewWithTextOrRaise(u'Headrest Fold')

com_volvocars_launcher___id_pager = vc.findViewByIdOrRaise("com.volvocars.launcher:id/pager")
com_volvocars_launcher___id_recycler_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/recycler_view")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_card_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/card_view")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_temperature_slider_pane_left = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_left")
com_volvocars_launcher___id_status_bar_clock = vc.findViewByIdOrRaise("com.volvocars.launcher:id/status_bar_clock")
com_volvocars_launcher___id_temperature_l_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_l_text")
com_volvocars_launcher___id_seat_l = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_l")
com_volvocars_launcher___id_seat_r = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_r")
com_volvocars_launcher___id_temperature_r_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_r_text")
com_volvocars_launcher___id_temperature_slider_pane_right = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_right")


# Setup and execute drag and drop with hold button, should move one square
# over and one square down
startX = hold_button.getX()+hold_button.getWidth()/2
startY = hold_button.getY()+hold_button.getHeight()/2
newX = startX+200
newY = startY+300
pressAndDrag(startX, startY, 3, newX, newY, 30)
time.sleep(1)

print("Toggle buttons once")
hold_button.touch()
charge_button.touch()
ext_sound_button.touch()
headrest_button.touch()
time.sleep(1)

print("Toggle buttons again")
hold_button.touch()
charge_button.touch()
ext_sound_button.touch()
headrest_button.touch()
time.sleep(1)

# Swip back to main view
vc.swipe(767, 100, 0, 100, 1000)
