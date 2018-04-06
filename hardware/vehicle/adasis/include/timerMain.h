/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <IDispatcher.h>
#include "adasisFlexrayConverterImpl.h"

using tarmac::eventloop::IDispatcher;
#define FOUREIGTHYTIMER 12
#define TWOFOURTYTIMER 6
#define TWOHUNDREDTIMER 5
#define RESET 0
#define FLEXRAYFRAME 40

class timerGoogleImpl {
    int fourEightyCounter = FOUREIGTHYTIMER;
    int twoHundredTimer = TWOHUNDREDTIMER;
    int twoFourtyTimer = TWOFOURTYTIMER;
    int seconds = RESET;
    adasisFlexrayConverterImpl* googleAdasisConverter;

  public:
    timerGoogleImpl(adasisFlexrayConverterImpl* googleAdasisConverterhal);
    void timeMessages();
};
