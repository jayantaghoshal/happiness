/**************************************************************************/
/* File:      desip.h                                                     */
/*                                                                        */
/* Copyright: 2005 Delphi Technologies, Inc., All Rights Reserved         */
/*                                                                        */
/* Description:                                                           */
/* Interface of the DESIP driver                                          */
/*                                                                        */
/* Revision history:                                                      */
/* 29 January 2016, Dieter Slowik                                         */
/*                                                                        */
/* Initial revision.                                                      */
/*                                                                        */
/**************************************************************************/

#ifndef DESIP_H
#define DESIP_H

/*
=============================================================================
  INCLUDE FILES
=============================================================================
*/
#include "reuse.h"
#include "hisip_router.h"

/*
=============================================================================
  DEFINITIONS
=============================================================================
*/

/* thread id and name of the interface DESIP/application */
#define DESIP_IF_THREAD_ID        ECD_IF_THREAD_ID+3
#define DESIP_IF_THREAD_NAME      "DESIP_IF_THREAD"
#define DESIP_WORKER_THREAD_ID   (ECD_IF_THREAD_ID + 4)
#define DESIP_READER_THREAD_ID   (ECD_IF_THREAD_ID + 5)

#define DESIP_LINK_IDX0          (0)
#define DESIP_LINK_IDX1          (1)
#define DESIP_LINK_IDX2          (2)

/*************************************************************************
 ***
 *** Desip-Interface-Functions to ECD 
 ***
 *************************************************************************
 ***/
ECD_Error_Codes open_Layer2Connection(ECD_Connection_State *Tx_State_ptr,
                                      ECD_Connection_State *Rx_State_ptr,
                                      const char           *pathname,
                                      int                  hwIndex,
                                      unsigned int         uartSpeed);

ECD_Error_Codes close_Layer2Connection(int hwIndex);

ECD_Error_Codes transmit_Layer2Connection(ROUTER_MESSAGE *out_msg, int desipLinkIndex);


/*************************************************************************
 ***
 *** Desip-Interface-Functions to Layer 1 / UART-Driver
 ***
 *************************************************************************
 ***/
void SIPOpen (int (*pGetChr) (int), int (*pPutChr) (int, int), int channelIdx);

int  SIPXmit (char *pBuf, int Len, int channelIdx);

#endif
