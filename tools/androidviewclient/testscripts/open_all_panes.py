#! /usr/bin/env python
# -*- coding: utf-8 -*-
'''
Copyright (C) 2013-2017  Diego Torres Milano
Created on 2017-06-13 by Culebra v13.3.0
                      __    __    __    __
                     /  \  /  \  /  \  /  \ 
____________________/  __\/  __\/  __\/  __\_____________________________
___________________/  /__/  /__/  /__/  /________________________________
                   | / \   / \   / \   / \   \___
                   |/   \_/   \_/   \_/   \    o \ 
                                           \_____/--<
@author: Diego Torres Milano
@author: Jennifer E. Swofford (ascii art snake)
'''


import re
import sys
import os

from com.dtmilano.android.viewclient import ViewClient
from vcc_utilities import *

TAG = 'CULEBRA'

_s = 1
_v = '--verbose' in sys.argv


kwargs1 = {'ignoreversioncheck': False, 'verbose': False, 'ignoresecuredevice': False}
device, serialno = ViewClient.connectToDeviceOrExit(**kwargs1)
kwargs2 = {'forceviewserveruse': False, 'useuiautomatorhelper': False, 'ignoreuiautomatorkilled': True, 'autodump': False, 'startviewserver': True, 'compresseddump': True}
vc = ViewClient(device, serialno, **kwargs2)

pressHomeButton()

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
com_volvocars_launcher___id_fan_man_auto = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fan_man_auto")
com_volvocars_launcher___id_seat_r = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_r")
com_volvocars_launcher___id_temperature_r_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_r_text")
com_volvocars_launcher___id_temperature_slider_pane_right = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_right")

vc.findViewWithTextOrRaise(u'Navigation').touch()
vc.sleep(_s)
vc.dump(window=-1)

no_id1 = vc.findViewByIdOrRaise("id/no_id/1")
com_volvocars_launcher___id_pager = vc.findViewByIdOrRaise("com.volvocars.launcher:id/pager")
com_volvocars_launcher___id_recycler_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/recycler_view")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_title = vc.findViewWithTextOrRaise(u'Navigation')
no_id6 = vc.findViewByIdOrRaise("id/no_id/6")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_listview_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/listview_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_temperature_slider_pane_left = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_left")
com_volvocars_launcher___id_status_bar_clock = vc.findViewByIdOrRaise("com.volvocars.launcher:id/status_bar_clock")
com_volvocars_launcher___id_temperature_l_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_l_text")
com_volvocars_launcher___id_seat_l = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_l")
com_volvocars_launcher___id_fanman_small = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fanman_small")
com_volvocars_launcher___id_fan_man_auto = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fan_man_auto")
com_volvocars_launcher___id_seat_r = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_r")
com_volvocars_launcher___id_temperature_r_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_r_text")
com_volvocars_launcher___id_temperature_slider_pane_right = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_right")

vc.findViewWithTextOrRaise(u'Media').touch()
vc.sleep(_s)
vc.dump(window=-1)

no_id1 = vc.findViewByIdOrRaise("id/no_id/1")
com_volvocars_launcher___id_pager = vc.findViewByIdOrRaise("com.volvocars.launcher:id/pager")
com_volvocars_launcher___id_recycler_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/recycler_view")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_toolbar_btn1 = vc.findViewByIdOrRaise("com.volvocars.launcher:id/toolbar_btn1")
com_volvocars_launcher___id_item_toolbar_btn_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_toolbar_btn_text")
com_volvocars_launcher___id_toolbar_btn2 = vc.findViewByIdOrRaise("com.volvocars.launcher:id/toolbar_btn2")
com_volvocars_launcher___id_item_toolbar_btn_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_toolbar_btn_text")
com_volvocars_launcher___id_toolbar_btn3 = vc.findViewByIdOrRaise("com.volvocars.launcher:id/toolbar_btn3")
com_volvocars_launcher___id_item_toolbar_btn_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_toolbar_btn_text")
com_volvocars_launcher___id_tile_media_nowplaying_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/tile_media_nowplaying_icon")
com_volvocars_launcher___id_media_queue_header = vc.findViewByIdOrRaise("com.volvocars.launcher:id/media_queue_header")
com_volvocars_launcher___id_media_queue_list = vc.findViewByIdOrRaise("com.volvocars.launcher:id/media_queue_list")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_listview_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/listview_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_temperature_slider_pane_left = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_left")
com_volvocars_launcher___id_status_bar_clock = vc.findViewByIdOrRaise("com.volvocars.launcher:id/status_bar_clock")
com_volvocars_launcher___id_temperature_l_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_l_text")
com_volvocars_launcher___id_seat_l = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_l")
com_volvocars_launcher___id_fanman_small = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fanman_small")
com_volvocars_launcher___id_fan_man_auto = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fan_man_auto")
com_volvocars_launcher___id_seat_r = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_r")
com_volvocars_launcher___id_temperature_r_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_r_text")
com_volvocars_launcher___id_temperature_slider_pane_right = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_right")

vc.findViewWithTextOrRaise(u'Phone').touch()
vc.sleep(_s)
vc.dump(window=-1)

no_id1 = vc.findViewByIdOrRaise("id/no_id/1")
com_volvocars_launcher___id_pager = vc.findViewByIdOrRaise("com.volvocars.launcher:id/pager")
com_volvocars_launcher___id_recycler_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/recycler_view")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_subtitle = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_subtitle")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_toolbar_btn1 = vc.findViewByIdOrRaise("com.volvocars.launcher:id/toolbar_btn1")
com_volvocars_launcher___id_item_toolbar_btn_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_toolbar_btn_text")
com_volvocars_launcher___id_toolbar_btn2 = vc.findViewByIdOrRaise("com.volvocars.launcher:id/toolbar_btn2")
com_volvocars_launcher___id_item_toolbar_btn_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_toolbar_btn_text")
com_volvocars_launcher___id_toolbar_btn3 = vc.findViewByIdOrRaise("com.volvocars.launcher:id/toolbar_btn3")
com_volvocars_launcher___id_item_toolbar_btn_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_toolbar_btn_text")
com_volvocars_launcher___id_toolbar_btn4 = vc.findViewByIdOrRaise("com.volvocars.launcher:id/toolbar_btn4")
com_volvocars_launcher___id_item_toolbar_btn_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_toolbar_btn_text")
com_volvocars_launcher___id_listview_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/listview_title")
com_volvocars_launcher___id_info_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/info_text")
com_volvocars_launcher___id_bottom_btn = vc.findViewByIdOrRaise("com.volvocars.launcher:id/bottom_btn")
com_volvocars_launcher___id_bottom_txt = vc.findViewByIdOrRaise("com.volvocars.launcher:id/bottom_txt")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_temperature_slider_pane_left = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_left")
com_volvocars_launcher___id_status_bar_clock = vc.findViewByIdOrRaise("com.volvocars.launcher:id/status_bar_clock")
com_volvocars_launcher___id_temperature_l_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_l_text")
com_volvocars_launcher___id_seat_l = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_l")
com_volvocars_launcher___id_fanman_small = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fanman_small")
com_volvocars_launcher___id_fan_man_auto = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fan_man_auto")
com_volvocars_launcher___id_seat_r = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_r")
com_volvocars_launcher___id_temperature_r_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_r_text")
com_volvocars_launcher___id_temperature_slider_pane_right = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_right")

vc.findViewWithTextOrRaise(u'Studio').touch()
vc.sleep(_s)
vc.dump(window=-1)

no_id1 = vc.findViewByIdOrRaise("id/no_id/1")
com_volvocars_launcher___id_pager = vc.findViewByIdOrRaise("com.volvocars.launcher:id/pager")
com_volvocars_launcher___id_recycler_view = vc.findViewByIdOrRaise("com.volvocars.launcher:id/recycler_view")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
com_volvocars_launcher___id_listview_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/listview_title")
com_volvocars_launcher___id_item_icon = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_icon")
com_volvocars_launcher___id_item_title = vc.findViewByIdOrRaise("com.volvocars.launcher:id/item_title")
no_id13 = vc.findViewByIdOrRaise("id/no_id/13")
com_volvocars_launcher___id_temperature_slider_pane_left = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_left")
com_volvocars_launcher___id_status_bar_clock = vc.findViewByIdOrRaise("com.volvocars.launcher:id/status_bar_clock")
com_volvocars_launcher___id_temperature_l_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_l_text")
com_volvocars_launcher___id_seat_l = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_l")
com_volvocars_launcher___id_fanman_small = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fanman_small")
com_volvocars_launcher___id_fan_man_auto = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fan_man_auto")
com_volvocars_launcher___id_seat_r = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_r")
com_volvocars_launcher___id_temperature_r_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_r_text")
com_volvocars_launcher___id_temperature_slider_pane_right = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_right")

vc.findViewWithTextOrRaise(u'Studio').touch()
vc.sleep(_s)
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
com_volvocars_launcher___id_fan_man_auto = vc.findViewByIdOrRaise("com.volvocars.launcher:id/fan_man_auto")
com_volvocars_launcher___id_seat_r = vc.findViewByIdOrRaise("com.volvocars.launcher:id/seat_r")
com_volvocars_launcher___id_temperature_r_text = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_r_text")
com_volvocars_launcher___id_temperature_slider_pane_right = vc.findViewByIdOrRaise("com.volvocars.launcher:id/temperature_slider_pane_right")

