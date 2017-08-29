
//#include <desip/desip.h>

#include "desip_handler.h"

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

#include "message_api.h"

#undef LOG_TAG
#define LOG_TAG "VSD"
#include <cutils/log.h>

static unsigned int convertToBaudrateIntType(const char *uartSpeed);

/*static void printStartupErrorMessage(ECD_Error_Codes error)
{
    log_error() << "Failed to start VSM with UART " << uartPath;

    switch (error)
    {
    case ECD_ERROR_NO_TX_CREATION:
    {
        log_error() << "Creation of Desip-Worker-Thread failed - exit main() of "
                       "vipcommunication";
        break;
    }
    case ECD_ERROR_NO_RX_CREATION:
    {
        log_error() << "Creation of Desip-Reader-Thread failed - exit main() of "
                       "vipcommunication";
        break;
    }
    case ECD_ERROR_NO_TX_CONNECTION:
    {
        log_error() << "Opening of LAYER1-Driver to UART failed - exit main() of "
                       "vipcommunication";
        break;
    }
    case ECD_ERROR_IPC_FAILURE:
    {
        log_error() << "Configuration of IPC failed - exit main() of vipcommunication";
        break;
    }
    default:
    {
        log_error() << "Unknown error = " << error << " in initDesipThreads() - exit main() of vipcommunication";
        break;
    }
    }
}*/

/*void messageReceive(ROUTER_MESSAGE *in_msg)
{
   avmpMessageInject((uint8_t *)in_msg->data, (uint32_t)in_msg->data_size);
}
*/

void messageSend(Message_Send_T *msg_data)
{
  /* ECD_Error_Codes error = ECD_ERROR_NO_ERROR;
   ROUTER_MESSAGE  out_msg;
   void*           payloadPointer;
   int             msg_size;
   char*           transmitBufferPtr;

   if (nullptr == msg_data)
   {
       log_warning() << "messageSend(): Bad input parameter";
       return;
   }

   msg_size          = msg_data->data_size;
   transmitBufferPtr = (char*)malloc(msg_size);
   if (transmitBufferPtr == NULL)
   {
       log_error() << "messageSend(): Unable to allocate Transmit-Buffer";
       return;
   }
   payloadPointer = transmitBufferPtr;

   // Fill Payload-Data into Message-PayLoad
   if (msg_data->data_size > 0)
   {
       // Message contains Header + Data
       log_verbose() << "messageSend(): Message to send contains Header + Data";
       memcpy(payloadPointer, msg_data->data_ptr, msg_data->data_size);
   }
   else
   {
       log_debug() << "messageSend(): Message to send contains only Header";
   }

   out_msg.data_size = msg_size;
   out_msg.data      = (void*)transmitBufferPtr;

   error = transmit_Layer2Connection(&out_msg, DESIP_LINK_IDX1);
   if (error != ECD_ERROR_NO_ERROR)
   {
       log_error() << "messageSend(): Unable to transmit data to Layer2-Component";
       free(transmitBufferPtr);
   }
   */
}

bool initDesip(const char *pathname, const char *uartSpeed)
{
  /*
 ECD_Error_Codes         result = ECD_ERROR_NO_ERROR;
 ECD_Connection_State tx_state  = ECD_CONNECTION_NO_CONNECTION;
 ECD_Connection_State rx_state  = ECD_CONNECTION_NO_CONNECTION;

 //==========================================================================
 //===  Open Connection to DESIP-Layer = Layer2
 //===  Open Connection to UART-Driver = Layer1
 //==========================================================================
 result = open_Layer2Connection(&tx_state, &rx_state, pathname, DESIP_LINK_IDX1,
                                convertToBaudrateIntType(uartSpeed));
 if((result != ECD_ERROR_NO_ERROR) || (tx_state == ECD_CONNECTION_NO_CONNECTION))
 {
    // DESIP initialization failed
    ALOGE ("initDesipThreads(): Creation of Layer2Connection-Threads failed!");
    result = open_Layer2Connection(&tx_state, &rx_state, pathname, DESIP_LINK_IDX1,
    printStartupErrorMessage(result);
    return false;
 }

 LOGI("initDesipThreads(): Opening HW-Device was successful");
*/
  return true;
}

static unsigned int convertToBaudrateIntType(const char *uartSpeed)
{

  // B115200 is a constant defined in termios.h 
  unsigned int uartBaudrate = B115200;

  ALOGI("initDesipThreads(): Setting UART speed = %s", uartSpeed);
  if (0 == strcmp(uartSpeed, "115200"))
  {
    uartBaudrate = B115200;
  }
  else if (0 == strcmp(uartSpeed, "230400"))
  {
    uartBaudrate = B230400;
  }
  else if (0 == strcmp(uartSpeed, "460800"))
  {
    uartBaudrate = B460800;
  }
  else if (0 == strcmp(uartSpeed, "921600"))
  {
    uartBaudrate = B921600;
  }
  else
  {
    ALOGW("initDesipThreads(): Couldn't find matching UART speed, defaulting to 115200");
    uartBaudrate = B115200;
  }

  return uartBaudrate;
}
