/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <desip_api.h>

#include "desip_handler.h"
#include "hisip_router.h"
#include "hisip_router_api.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <termios.h>
#include <unistd.h>
#include "getopt.h"

#include <cutils/log.h>
#include "message_api.h"
#undef LOG_TAG
#define LOG_TAG "VSD"

static void printStartupErrorMessage(ECD_Error_Codes error) {
    switch (error) {
        case ECD_ERROR_NO_ERROR: {
            break;
        }
        case ECD_ERROR_NO_CONNECTION_ESTABLISHED: {
            ALOGE("ECD_ERROR_NO_CONNECTION_ESTABLISHED");
            break;
        }
        case ECD_ERROR_NO_CALLBACK_FUNCTION: {
            ALOGE("ECD_ERROR_NO_CALLBACK_FUNCTION");
            break;
        }
        case ECD_ERROR_CONNECTION_ALREADY_ESTABLISHED: {
            ALOGE("ECD_ERROR_CONNECTION_ALREADY_ESTABLISHED");
            break;
        }
        case ECD_ERROR_TRANSMIT: {
            ALOGE("ECD_ERROR_TRANSMIT");
            break;
        }
        case ECD_ERROR_HW_FD_CLOSE: {
            ALOGE("ECD_ERROR_HW_FD_CLOSE");
            break;
        }
        default: {
            ALOGE("Unknown error = %d ", error);
            break;
        }
    }
}

void messageReceive(ROUTER_MESSAGE *in_msg) { avmpMessageInject((uint8_t *)in_msg->data, (uint32_t)in_msg->data_size); }

void messageSend(Message_Send_T *msg_data) {
    if (nullptr == msg_data) {
        ALOGW("messageSend(): Bad input parameter");
        return;
    }

    int msg_size = msg_data->data_size;
    uint8_t *transmitBufferPtr = (uint8_t *)malloc(msg_size);
    if (transmitBufferPtr == NULL) {
        ALOGE("messageSend(): Unable to allocate Transmit-Buffer");
        return;
    }
    void *payloadPointer = transmitBufferPtr;

    // Fill Payload-Data into Message-PayLoad
    if (msg_data->data_size > 0) {
        // Message contains Header + Data
        ALOGV("messageSend(): Message to send contains Header + Data");
        memcpy(payloadPointer, msg_data->data_ptr, msg_data->data_size);
    } else {
        ALOGV("messageSend(): Message to send contains only Header");
    }

    ROUTER_MESSAGE out_msg;
    out_msg.data_size = msg_size;
    out_msg.data = transmitBufferPtr;

    ECD_Error_Codes error = transmit_Layer2Connection(&out_msg);
    if (error != ECD_ERROR_NO_ERROR) {
        ALOGE("messageSend(): Unable to transmit data to Layer2-Component");
        free(transmitBufferPtr);
    }
}

bool initDesip(const char *pathname) {
    ECD_Connection_State tx_state = ECD_CONNECTION_NO_CONNECTION;
    ECD_Connection_State rx_state = ECD_CONNECTION_NO_CONNECTION;

    //==========================================================================
    //===  Open Connection to DESIP-Layer = Layer2
    //===  Open Connection to UART-Driver = Layer1
    //==========================================================================
    setCallback_Layer2Connection(messageReceive);
    ECD_Error_Codes result = open_Layer2Connection(&tx_state, &rx_state, pathname);
    if ((result != ECD_ERROR_NO_ERROR) || (tx_state == ECD_CONNECTION_NO_CONNECTION)) {
        // DESIP initialization failed
        ALOGE("initDesipThreads(): Creation of Layer2Connection-Threads failed using UART: %s", pathname);
        printStartupErrorMessage(result);
        return false;
    }

    ALOGI("initDesipThreads(): Opening HW-Device was successful");

    return true;
}
