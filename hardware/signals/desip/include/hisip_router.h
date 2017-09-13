/**************************************************************************/
/* File:      hisip_router.h                                              */
/*                                                                        */
/* Copyright: 2016 Delphi Technologies, Inc., All Rights Reserved         */
/*                                                                        */
/* Description:                                                           */
/* Interface of the external communication driver                         */
/*                                                                        */
/* Revision history:                                                      */
/* 05. February 2016                                                      */
/*   Initial revision.                                                    */
/*                                                                        */
/**************************************************************************/

#ifndef HISIP_ROUTER_H
#define HISIP_ROUTER_H

/*
=============================================================================
  INCLUDE FILES
=============================================================================
*/
#include "hisip_router_api.h"

/*
=============================================================================
  DEFINITIONS
=============================================================================
*/

typedef enum
{
    ECD_ERROR_NO_ERROR = 0,
    ECD_ERROR_NO_MEMORY,
    ECD_ERROR_UNKNOWN_APP_ID,
    ECD_ERROR_NOT_SUBSCRIBED,
    ECD_ERROR_NO_TX_CREATION,
    ECD_ERROR_NO_RX_CREATION,
    ECD_ERROR_NO_TX_CONNECTION,
    ECD_ERROR_NO_RX_CONNECTION,
    ECD_ERROR_TRANSMIT,
    ECD_ERROR_NO_HW_AVAILABLE,
    ECD_ERROR_IPC_FAILURE,
    ECD_ERROR_LAST
}
ECD_Error_Codes;

typedef enum
{
    ECD_CONNECTION_NO_CONNECTION = 0,
    ECD_CONNECTION_TX_CONNECTION,
    ECD_CONNECTION_RX_CONNECTION
}
ECD_Connection_State;

typedef struct ECD_RxTx_Connection
{
    ECD_Connection_State *Tx_State_ptr;
    ECD_Connection_State *Rx_State_ptr;
    const char           *TxRx_path_ptr;
}
ECD_RxTx_Connection;

/***
 ***  Function: initDesipThreads
 ***/
ECD_Error_Codes initDesipThreads(const char *pathname, const char *uartSpeed);

/***
 ***  Function: handleTestMode01()
 ***/
void handleTestMode01();

#endif
