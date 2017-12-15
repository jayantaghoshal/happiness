/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "tempimpl.h"
#include <cutils/log.h>
#include "Application_dataelement.h"

#undef LOG_TAG
#define LOG_TAG "TemperatureImpl"

using namespace ApplicationDataElement;
using namespace autosar;

TempImpl::TempImpl(NotifiableProperty<float>& temp_left, NotifiableProperty<float>& temp_right)
    : m_temp_left(temp_left), m_temp_right(temp_right) {
    setleftTemp(23);
    setrightTemp(23);
}

void TempImpl::setleftTemp(float temp_left) {
    ALOGI("%s: Send HmiCmptmtTSp_info for setting left temperature to: %f", __FUNCTION__, temp_left);
    HmiCmptmtTSp temperatureSignal;
    if (temp_left < 17.0) {
        temperatureSignal.HmiCmptmtTSpSpclForRowFirstLe = HmiCmptmtTSpSpcl::Lo;
        temperatureSignal.HmiCmptmtTSpForRowFirstLe = 17;
    }

    else if (temp_left > 27.0) {
        temperatureSignal.HmiCmptmtTSpSpclForRowFirstLe = HmiCmptmtTSpSpcl::Hi;
        temperatureSignal.HmiCmptmtTSpForRowFirstLe = 27;
    }

    else {
        temperatureSignal.HmiCmptmtTSpSpclForRowFirstLe = HmiCmptmtTSpSpcl::Norm;
        temperatureSignal.HmiCmptmtTSpForRowFirstLe = temp_left;
    }

    HmiCmptmtTSpSender.send(temperatureSignal);
    m_temp_left.set(static_cast<float>(temperatureSignal.HmiCmptmtTSpForRowFirstLe));
}

void TempImpl::setrightTemp(float temp_right) {
    ALOGI("%s: Send HmiCmptmtTSp_info for setting right temperature to: %f", __FUNCTION__, temp_right);
    HmiCmptmtTSp temperatureSignal;
    if (temp_right < 17.0) {
        temperatureSignal.HmiCmptmtTSpSpclForRowFirstLe = HmiCmptmtTSpSpcl::Lo;
        temperatureSignal.HmiCmptmtTSpForRowFirstRi = 17;
    }

    else if (temp_right > 27.0) {
        temperatureSignal.HmiCmptmtTSpSpclForRowFirstLe = HmiCmptmtTSpSpcl::Hi;
        temperatureSignal.HmiCmptmtTSpForRowFirstRi = 27;
    }

    else {
        temperatureSignal.HmiCmptmtTSpSpclForRowFirstLe = HmiCmptmtTSpSpcl::Norm;
        temperatureSignal.HmiCmptmtTSpForRowFirstRi = temp_right;
    }

    HmiCmptmtTSpSender.send(temperatureSignal);
    m_temp_right.set(static_cast<float>(temperatureSignal.HmiCmptmtTSpForRowFirstRi));
}