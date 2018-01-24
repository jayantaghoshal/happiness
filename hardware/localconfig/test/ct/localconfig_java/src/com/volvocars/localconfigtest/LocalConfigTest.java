/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

package com.volvocars.localconfigtest;

import android.support.test.runner.AndroidJUnit4;
import android.support.test.rule.ActivityTestRule;
import android.support.test.filters.SmallTest;

import org.junit.runner.RunWith;
import org.junit.Test;
import org.junit.Rule;
import java.lang.String;

@SmallTest
@RunWith(AndroidJUnit4.class)
public class LocalConfigTest extends ActivityTestRule<MainActivity> {

  @Rule
  public ActivityTestRule<MainActivity> mActivityRule =
      new ActivityTestRule<>(MainActivity.class);

  public LocalConfigTest() { super(MainActivity.class); }

  @Test
  public void testLocalConfigEnvPath() throws Exception {

    mActivityRule.getActivity().LocalConfigEnvPath();
  }

  @Test
  public void testLocalConfigMockedPathExists() throws Exception {

    mActivityRule.getActivity().LocalConfigMockedPathExists();
  }

  @Test
  public void testLocalConfigConnection() throws Exception {

    mActivityRule.getActivity().LocalConfigConnection();
  }

  @Test
  public void testLocalConfigDoubleConnection() throws Exception {

    mActivityRule.getActivity().LocalConfigDoubleConnection();
  }

  @Test
  public void testLocalConfigGetInteger() throws Exception {

    mActivityRule.getActivity().LocalConfigGetInteger();
  }
  @Test
  public void testLocalConfigGetDouble() throws Exception {

    mActivityRule.getActivity().LocalConfigGetDouble();
  }
  @Test
  public void testLocalConfigGetString() throws Exception {

    mActivityRule.getActivity().LocalConfigGetString();
  }

  @Test
  public void testLocalConfigGetWrongString() throws Exception {
    mActivityRule.getActivity().LocalConfigGetWrongString();
  }

  @Test
  public void testLocalConfigGetWrongInteger() throws Exception {
    mActivityRule.getActivity().LocalConfigGetWrongInteger();
  }

  @Test
  public void testLocalConfigGetWrongDouble() throws Exception {
    mActivityRule.getActivity().LocalConfigGetWrongDouble();
  }
}