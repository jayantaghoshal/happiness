/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.volvocars.test.lib.vehiclehal;

import static java.lang.Integer.toHexString;

/**
 * This exception is raised when IVehicle#get or IVehicle#set returns StatusCode.TRY_AGAIN. This
 * usually happens during boot-up meaning that Vehicle HAL is not ready to get or set that property.
 */
public class PropertyTimeoutException extends Exception {
    PropertyTimeoutException(int property) {
        super("Property 0x" + toHexString(property) + " is not ready yet.");
    }
}
