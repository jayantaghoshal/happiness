/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <binder/ProcessState.h>
#include <unistd.h>
#include <cstdint>
#include "carconfig_updater.h"

// DO NOT CHANGE THE ORDER!
#define DTS_PRIVATE_MODEL Tr_Declare_App(CarConfigUpdater)
#include "DTS_GenericPrivateModel.h"
#define DUMMY_TO_NOT_MAKE_CLANG_TIDY_SORT  // clang-tidy off does not work on include sorting
#include "DTS_DeclarePrivateModel.h"
// END OF: DO NOT CHANGE THE ORDER!

int32_t main(int argc, char* argv[]) {
    android::ProcessState::initWithDriver("/dev/vndbinder");
    return CarConfigUpdater::runUpdater();
}
