/**************************************************************************/
/* File:      hisip_router_api.h                                          */
/*                                                                        */
/* Copyright: 2016 Delphi Technologies, Inc., All Rights Reserved         */
/*                                                                        */
/* Description:                                                           */
/* HISIP-Message description                                              */
/* Based on following Files in former ECD-Building Block                  */
/* - desip_message.h                                                      */
/* - xsal_data_types.h                                                    */
/*                                                                        */
/* Revision history:                                                      */
/*                                                                        */
/* 03 February 2016, Dieter Slowik                                        */
/*   Initial revision.                                                    */
/*                                                                        */
/**************************************************************************/

#ifndef HISIP_ROUTER_API_H
#define HISIP_ROUTER_API_H

/****************************************************************************
 ***
 ***       Defines for DESIP-Message-Queue
 ***
 ****************************************************************************/
#define  DESIP_MSG_QUEUE_Port0         "/dev/socket/DESIPQueuePort_0"
#define  DESIP_MSG_QUEUE_Port1         "/dev/socket/DESIPQueuePort_1"

#define  DESIP_MSG_QUEUE_MAX_NUMBER         128
#define  DESIP_EVENT_RECEIVE_DATA_PORT_0    0x01
#define  DESIP_EVENT_TRANSMIT_DATA_PORT_0   0x02
#define  DESIP_EVENT_RECEIVE_DATA_PORT_1    0x03
#define  DESIP_EVENT_TRANSMIT_DATA_PORT_1   0x04
#define  HISIP_MESSAGE_HEADER_SIZE          4

/****************************************************************************
 ***
 ***       Defines for HISIP-Header
 ***
 ****************************************************************************/

typedef enum
{
    NODE_ID_BC                = 0x00,
    NODE_ID_AP                = 0x01,
    NODE_ID_VIP               = 0x02,
    NODE_ID_DSP               = 0x03,
    NODE_ID_HKP               = 0x04,
    NODE_ID_UIC               = 0x05
}
NODE_ID;

/****************************************************************************
 ***
 ***       Data-Structures for HISIP-Header
 ***
 ****************************************************************************/
typedef struct
{
    uint8_t  tnid;              /* target node id */
    uint8_t  snid;              /* source node id */
    uint8_t  application_id;
    uint8_t  function_id;
    uint8_t  data[1];           /* place holder, real length is msg specific */
}
HISIP_MESSAGE;


/****************************************************************************
 ***
 ***       Data-Structures for HISIP-Router-IPC
 ***
 ****************************************************************************/
typedef struct
{
   /** Size of data associated with the message */
   size_t data_size;

   /** Pointer to message data */
   void *data;

   /** Event ID */
   int32_t event_id;
}
ROUTER_MESSAGE;

void messageReceive(ROUTER_MESSAGE *in_msg);

#endif
