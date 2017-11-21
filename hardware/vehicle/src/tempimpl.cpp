#include "tempimpl.h"
#include "Application_dataelement.h"

using namespace ApplicationDataElement;
using namespace autosar;

TempImpl::TempImpl(NotifiableProperty<float>& temp_left) : m_temperature(temp_left) { setleftTemp(23); }

HmiCmptmtTSp temperatureSignal;
HmiCmptmtTSpSpcl tmtMode = HmiCmptmtTSpSpcl::Norm;

void TempImpl::setleftTemp(float temp_left) {
    if (temp_left < 16.5) {
        tmtMode = HmiCmptmtTSpSpcl::Lo;
        m_temperature.set(temp_left);
        temperatureSignal.HmiCmptmtTSpForRowFirstLe = 16.5;
    }

    else if (temp_left > 27.5) {
        tmtMode = HmiCmptmtTSpSpcl::Hi;
        temperatureSignal.HmiCmptmtTSpForRowFirstLe = 27.5;
    }

    else {
        tmtMode = HmiCmptmtTSpSpcl::Norm;
        temperatureSignal.HmiCmptmtTSpForRowFirstLe = temp_left;
    }

    temperatureSignal.HmiCmptmtTSpSpclForRowFirstLe = tmtMode;
    HmiCmptmtTSpSender.send(temperatureSignal);
}

void TempImpl::setrightTemp(float temp_right) {
    if (temp_right < 16.5) {
        tmtMode = HmiCmptmtTSpSpcl::Lo;
        temperatureSignal.HmiCmptmtTSpForRowFirstRi = 16.5;
    }

    else if (temp_right > 27.5) {
        tmtMode = HmiCmptmtTSpSpcl::Hi;
        temperatureSignal.HmiCmptmtTSpForRowFirstRi = 27.5;
    }

    else {
        tmtMode = HmiCmptmtTSpSpcl::Norm;
        temperatureSignal.HmiCmptmtTSpForRowFirstRi = temp_right;
    }

    temperatureSignal.HmiCmptmtTSpSpclForRowFirstRi = tmtMode;
    HmiCmptmtTSpSender.send(temperatureSignal);
}