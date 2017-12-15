/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef VSM_H
#define VSM_H

// Use this function to initialize the vehicle signals manager
void vsm_init(void);

// Use this function to de-initialize the vehicle signals manager
void vsm_deInit(void);

// This function should be called when the lower layers has been setup and
// is ready for communication
void vsm_start(void);

#endif  // VSM_H
