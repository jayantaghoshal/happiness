/***********************************************************************
*       (c) '2005' Delphi Delco Electronics Systems
*            All rights Reserved.  Delphi D confidential
***********************************************************************/
/***********************************************************************
*
*  Title:              desip_main.cpp
*
*  Description:        DESIP-VIB driver main functions
*
*  Created:            29.01.2016
*
*  Author:             Dieter Slowik
*
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/un.h>


#undef LOG_TAG
#define LOG_TAG "DESIP"
#include <cutils/log.h>

#include "desip.h"
#include "desip_prot_if.h"
#include "vibdrvHWlink.h"

// Global Defines -----------------------------------
#ifndef EOK
#define EOK 0
#endif

#define MSG_INPUT_BUFFER_SIZE      50

//Define the DataType for Pointer to Thread-Functions
typedef void *THREADFKT(void *parameter);


typedef struct
{
   int                 connectionIndex;
   //----- Semaphore-Data ------------------
   sem_t               worker_thread_sem;
   sem_t               reader_thread_sem;
   unsigned int        sem_start;
   int                 pshared;
   //----- Queue-Data ---------------------
   char                QueueNameBuffer[50];
   char                *OwnQueueName;
   int                 QueueID;
   int                 QueueIDSend;
   int                 QueueIDReader;
   //----- Thread-Data --------------------
   pthread_t           thread_id_worker;
   pthread_t           thread_id_reader;
   pthread_attr_t      attr_worker_thread;
   pthread_attr_t      attr_reader_thread;
   THREADFKT           *p_ReaderThreadfunction;
   THREADFKT           *p_WorkerThreadfunction;
   //----- Connection-Status --------------
   ECD_RxTx_Connection Connection_State;
   //----- UART speed ---------------------
   unsigned int        uartSpeed;
}Layer2Connection;

// Global and static variables for this module
static int       Port_Reserved_Flags[NUM_HW_LINKS] = {0};
static int       workerThreadRunning[NUM_HW_LINKS] = {0};
static int       readerThreadRunning[NUM_HW_LINKS] = {0};
Layer2Connection connections[NUM_HW_LINKS];
extern volatile int       errno;

static void *worker_thread_port0(void* para);
static void *worker_thread_port1(void* para);
static void *reader_thread_port0(void *para);
static void *reader_thread_port1(void *para);
static void workerThreadDeSIPHandler(unsigned char *ReceivePtr, int hwLinkIndex);
static bool initConnection(int index, ECD_RxTx_Connection *p_ConnectionState, unsigned int uartSpeed);
static int  open_vib_driver(const char *pathname, int hwLinkIndex);
static inline bool messageReceived(int *max_sd, int listen_sd, fd_set *master_set, fd_set *working_set,
                                   char   *message, ssize_t expectedMessageLenght, timeval timeout);

/*===========================================================================
 *
 *  Function Name:  ECD_Error_Codes open_Layer2Connection()
 *
 ****************************************************************************
 *
 *  @brief          This function creates the Lazer2-Threads and
 *                  triggers the opening of DESIP-Modul and DESIP-Driver
 *
 *                  This function is running in Main-Thread of
 *                  vehicledatadriver-Process
 *
 ****************************************************************************
 *
 *  @post
 *
 *===========================================================================*/
ECD_Error_Codes open_Layer2Connection(ECD_Connection_State *Tx_State_ptr,
                                      ECD_Connection_State *Rx_State_ptr,
                                      const char           *pathname,
                                      int                  hwIndex,
                                      unsigned int         uartSpeed)
{
   ECD_Error_Codes     error = ECD_ERROR_NO_ERROR;
   pthread_attr_t      *attr_worker_thread_ptr;
   pthread_attr_t      *attr_reader_thread_ptr;
   ECD_RxTx_Connection Connection_State;
   size_t              stackSize = 16 * PTHREAD_STACK_MIN;
   int                 result;
   int                 sem_return;
   int                 errvalue;
   int                 on = 1;
   struct              sockaddr_un addr;

   /*------------------------------------------------------------------------*/
   /*   Fill Structure ECD_RxTx_Connection with Connection-State-Pointer     */
   /*------------------------------------------------------------------------*/
   Connection_State.Tx_State_ptr  = Tx_State_ptr;
   Connection_State.Rx_State_ptr  = Rx_State_ptr;
   Connection_State.TxRx_path_ptr = pathname;

   /*------------------------------------------------------------------------*/
   /* Check for available Ports                                              */
   /* Set  related Connection-Index                                          */
   /* Init related Connection                                                */
   /*------------------------------------------------------------------------*/
   if(Port_Reserved_Flags[hwIndex] != 0)
   {
      ALOGE("open_Layer2Connection(): All available UART-Ports are busy - no new connection possible");
      error = ECD_ERROR_NO_HW_AVAILABLE;
      return (error);
   }
   else
   {
      if(false == initConnection(hwIndex, &Connection_State, uartSpeed))
      {
         error = ECD_ERROR_NO_HW_AVAILABLE;
         return (error);
      }
      Port_Reserved_Flags[hwIndex] = 1;
   }

   /*------------------------------------------------------------------------*/
   /* Create unnamed Semaphore to synchronize Worker-Thread <--> Main-Thread */
   /*------------------------------------------------------------------------*/
   sem_return = sem_init(&connections[hwIndex].worker_thread_sem, connections[hwIndex].pshared,
                          connections[hwIndex].sem_start);
   if(sem_return != 0)
   {
      ALOGE("open_Layer2Connection(): Init of worker_thread_sem for UART-Port %d  failed - errorCode = %d",hwIndex,sem_return);
      Port_Reserved_Flags[hwIndex] = 0;
      error = ECD_ERROR_NO_TX_CREATION;
      return (error);
   }

    /*-----------------------------------------------------------------------*/
    /* Create Semaphore for synchronization Reader-Thread <--> Main-Thread   */
    /*-----------------------------------------------------------------------*/
    sem_return = sem_init(&connections[hwIndex].reader_thread_sem, connections[hwIndex].pshared,
                           connections[hwIndex].sem_start);
    if(sem_return != 0)
    {
       ALOGE("open_Layer2Connection(): Init of reader_thread_sem for UART-Port %d errorCode = %d",hwIndex,sem_return);
       Port_Reserved_Flags[hwIndex] = 0;
       error = ECD_ERROR_NO_TX_CREATION;
       return (error);
    }

    /*-----------------------------------------------------------------------*/
    /* Create Message Queue                                                  */
    /*-----------------------------------------------------------------------*/
    // Remove already existing Queue
    errno = EOK;
    result = unlink(connections[hwIndex].OwnQueueName);
    if (result < 0)
    {
       errvalue = errno;
       if(errvalue == ENOENT)
       {

          ALOGI("No DeSIP-Message-Queue for connection %d  created so far",hwIndex);
       }
       else
       {
          ALOGE("open_Layer2Connection(): mq_unlink for connection %d returned with ErrorNumber = %d", hwIndex,errvalue);
       }
    }

    if ( (connections[hwIndex].QueueID = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
       ALOGE("open_Layer2Connection socket(): socket error");
       return (error);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    if (*connections[hwIndex].OwnQueueName == '\0') {
       *addr.sun_path = '\0';
       strncpy(addr.sun_path+1, connections[hwIndex].OwnQueueName+1, sizeof(addr.sun_path)-2);
    } else {
        strncpy(addr.sun_path, connections[hwIndex].OwnQueueName, sizeof(addr.sun_path)-1);
    }

    /*************************************************************/
    /* Allow socket descriptor to be reuseable                   */
    /*************************************************************/

    int rc = setsockopt(connections[hwIndex].QueueID, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
    if (rc < 0)
    {
      perror("setsockopt() failed");
      close(connections[hwIndex].QueueID);
      exit(-1);
    }

    /*************************************************************/
    /* Set socket to be nonblocking. All of the sockets for    */
    /* the incoming connections will also be nonblocking since  */
    /* they will inherit that state from the listening socket.   */
    /*************************************************************/
    rc = ioctl(connections[hwIndex].QueueID, FIONBIO, (char *)&on);
    if (rc < 0)
    {
      perror("ioctl() failed");
      close(connections[hwIndex].QueueID);
      exit(-1);
    }

    errno = EOK;
    if (bind(connections[hwIndex].QueueID , (struct sockaddr*)&addr, sizeof(addr)) == -1) {
         ALOGE("bind error, errno = %s",strerror(errvalue));
         return (error);
    }

    if (listen(connections[hwIndex].QueueID , 5) == -1) {
        ALOGE("listen error, errno = %s", strerror(errvalue));
        return (error);
    }

    if(connections[hwIndex].QueueID < 0)
    {
       errvalue = errno;
       ALOGE("open_Layer2Connection(): Creating of own Message-Queue returned with ErrorNumber = %d",errvalue);
       Port_Reserved_Flags[hwIndex] = 0;
       error = ECD_ERROR_NO_TX_CREATION;
       return error;
    }

   /*------------------------------------------------------------------------*/
   /*   Creation of Worker-Thread                                            */
   /*   1. Parameter = Pointer to thread_id, set in create-Function          */
   /*   2. Parameter = Pointer to Thread-Attributes                          */
   /*   3. Parameter = Function-Pointer to Thread-Function                   */
   /*   4. Parameter = Pointer to Thread-Arguments                           */
   /*------------------------------------------------------------------------*/
   attr_worker_thread_ptr = &connections[hwIndex].attr_worker_thread;
   //Set Default-Attributes for worker-Thread
   result = pthread_attr_init(attr_worker_thread_ptr);
   if(result != 0)
   {
      Port_Reserved_Flags[hwIndex] = 0;
      error = ECD_ERROR_NO_TX_CREATION;
   }
   else
   {
      // Set thread stack size
      result = pthread_attr_setstacksize(attr_worker_thread_ptr, stackSize);
      if(result != 0)
      {
         //Error: Creation of worker thread was not successful
         Port_Reserved_Flags[hwIndex] = 0;
         error = ECD_ERROR_NO_TX_CREATION;
      }
      else
      {
         //Call the Thread-Create-Function
         result = pthread_create(&connections[hwIndex].thread_id_worker, attr_worker_thread_ptr,
                                  connections[hwIndex].p_WorkerThreadfunction,
                                  (void *) &connections[hwIndex]);
         if(result != 0)
         {
            ALOGE("Creation of worker_thread for connection was not successful!");
            Port_Reserved_Flags[hwIndex] = 0;
            error = ECD_ERROR_NO_TX_CREATION;
         }
      }
   }

   /*------------------------------------------------------------------------*/
   /*     Wait for Worker-Thread-Semaphore                                   */
   /*------------------------------------------------------------------------*/
   errno = EOK;
   if(error == ECD_ERROR_NO_ERROR)
   {
      if(sem_wait(&connections[hwIndex].worker_thread_sem) < 0)
      {
         errvalue = errno;
         ALOGE("open_Layer2Connection(): Wait for worker thread semaphore returned with ErrorNumber = %d Meaning of this errno: %s",errvalue,strerror(errvalue));
         Port_Reserved_Flags[hwIndex] = 0;
         error = ECD_ERROR_NO_TX_CREATION;
      }
   }

   if(0 == workerThreadRunning[hwIndex])
   {
      // Worker thread failed to start correctly, no point in starting reader thread
      error = ECD_ERROR_NO_TX_CREATION;
   }

   /*------------------------------------------------------------------------*/
   /*   Creation of Reader-Thread                                            */
   /*------------------------------------------------------------------------*/
   if(error == ECD_ERROR_NO_ERROR)
   {
       attr_reader_thread_ptr = &connections[hwIndex].attr_reader_thread;
       //Set Default-Attributes for worker-Thread
       result = pthread_attr_init(attr_reader_thread_ptr);
       if(result != 0)
       {
          Port_Reserved_Flags[hwIndex] = 0;
          error = ECD_ERROR_NO_RX_CREATION;
       }
       else
       {
          // Set thread stack size
          result = pthread_attr_setstacksize(attr_reader_thread_ptr, stackSize);
          if(result != 0)
          {
             //Error: Creation of reader_thread was not successful
             Port_Reserved_Flags[hwIndex] = 0;
             error = ECD_ERROR_NO_RX_CREATION;
          }
          else
          {
             //Call the Thread-Create-Function
             result = pthread_create(&connections[hwIndex].thread_id_reader, attr_reader_thread_ptr,
                                      connections[hwIndex].p_ReaderThreadfunction,
                                      (void *) &connections[hwIndex]);
             if(result != 0)
             {
                ALOGE("Creation of reader_thread was not successful");
                Port_Reserved_Flags[hwIndex] = 0;
                error = ECD_ERROR_NO_RX_CREATION;
             }
          }
       }
   } //End of if(error == ECD_ERROR_NO_ERROR)

   /*------------------------------------------------------------------------*/
   /*     Wait for reader thread semaphore                                   */
   /*------------------------------------------------------------------------*/
   errno = EOK;
   if(error == ECD_ERROR_NO_ERROR)
   {
      if(sem_wait(&connections[hwIndex].reader_thread_sem) < 0)
      {
         errvalue = errno;
         ALOGE("open_Layer2Connection(): Wait for reader thread semaphore for UART-Port %d returned with ErrorNumber = %d",hwIndex,errvalue);
         *Rx_State_ptr = ECD_CONNECTION_NO_CONNECTION;
         Port_Reserved_Flags[hwIndex] = 0;
      }
   }

   //Return to Calling Function
   return (error);
}


/*===========================================================================
 *
 *  Function Name:  ECD_Error_Codes close_Layer2Connection()
 *
 ****************************************************************************
 *
 * @brief
 *
 ****************************************************************************
 *
 * @post
 *
 *===========================================================================*/
ECD_Error_Codes close_Layer2Connection(int hwIndex)
{
   ECD_Error_Codes error = ECD_ERROR_NO_ERROR;

    /* Put the threads to sleep */
   workerThreadRunning[hwIndex] = 0;
   readerThreadRunning[hwIndex] = 0;

   VIBMemoryUnMap(hwIndex);

   Port_Reserved_Flags[hwIndex] = 0;

   return (error);
}


/*===========================================================================
 *
 *  Function Name:  ECD_Error_Codes transmit_Layer2Connection()
 *
 ****************************************************************************
 *
 * @brief           This function returns after sending
 *                  SIP-Message to Desip-StateMachine via
 *                  sending Message to Desip-Worker-Thread
 *                  ==>
 *                  Context-Switch from Application-Thread to
 *                  Desip-Worker-Thread
 *
 ****************************************************************************
 *
 * @post
 *
 *===========================================================================*/
ECD_Error_Codes transmit_Layer2Connection(ROUTER_MESSAGE *out_msg, int desipLinkIndex)
{
   ECD_Error_Codes  error         = ECD_ERROR_NO_ERROR;
   size_t           out_data_size = out_msg->data_size;
   void             *p_out_data   = out_msg->data;
   int            msg_len       = sizeof(ROUTER_MESSAGE);
   int              errvalue;

   /*--------------------------------------------------------------------*/
   /*  Check if valid Data are available                                 */
   /*--------------------------------------------------------------------*/
   if( (p_out_data != NULL) && (out_data_size > 0) )
   {
      /*-----------------------------------------------------------------*/
      /*   Open Worker-Thread-MessageQueue                               */
      /*-----------------------------------------------------------------*/
      errno = EOK;
      if(connections[desipLinkIndex].QueueIDSend < 0)
      {
         ALOGV("transmit_Layer2Connection(): Opening transmit Message-Queue for connection %d",desipLinkIndex);
         if ( (connections[desipLinkIndex].QueueIDSend = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
         {
            ALOGE("transmit_Layer2Connection socket error");
            return ECD_ERROR_TRANSMIT;
         }

         struct sockaddr_un addr;
         memset(&addr, 0, sizeof(addr));
         addr.sun_family = AF_UNIX;
         strncpy(addr.sun_path, connections[desipLinkIndex].QueueNameBuffer, sizeof(addr.sun_path)-1);

         if (connect(connections[desipLinkIndex].QueueIDSend, (struct sockaddr*)&addr, sizeof(addr)) == -1)
         {
            ALOGE("transmit_Layer2Connection connect() socket error");
            return ECD_ERROR_TRANSMIT;
         }
      }

      if(connections[desipLinkIndex].QueueIDSend < 0)
      {
         errvalue = errno;
         error    = ECD_ERROR_TRANSMIT;
         ALOGE("transmit_Layer2Connection(): Open Message-Queue for connection %d returned with ErrorNumber = %d",desipLinkIndex,errvalue);
      }
      else
      {
         /*- ------------------------------------------------------------*/
         /* Trigger Context-Switch to DESIP-Worker-Thread                */
         /* Data are already available in out_msg                        */
         /*--------------------------------------------------------------*/
         if(DESIP_LINK_IDX0 == desipLinkIndex)
         {
            out_msg->event_id = DESIP_EVENT_TRANSMIT_DATA_PORT_0;
         }
         else if(DESIP_LINK_IDX1 == desipLinkIndex)
         {
            out_msg->event_id = DESIP_EVENT_TRANSMIT_DATA_PORT_1;
         }
         else
         {
            ALOGE("transmit_Layer2Connection(): Link %d is not supported",desipLinkIndex);
            return ECD_ERROR_TRANSMIT;
         }

         /*--------------------------------------------------------------*/
         /* Message contains reference to Transmit-Data                  */
         /* Message contains Data-Size-Info                              */
         /* Message contains Event-Info                                  */
         /*------------------------------------------------------   -----*/
         ALOGV("transmit_Layer2Connection(): writing data size = %d onto socket %s",msg_len,connections[desipLinkIndex].QueueNameBuffer);

         errno = EOK;
         if (write(connections[desipLinkIndex].QueueIDSend, out_msg, msg_len) != msg_len)
         {
            errvalue = errno;
            error    = ECD_ERROR_TRANSMIT;
            ALOGE("transmit_Layer2Connection(): mq_send() returned with ErrorNumber = %d error str = %s",errvalue,strerror(errvalue));
            
         }
         ALOGV("transmit_Layer2Connection(): MESSAGE WRITTEN!");
      }
   }
   else
   {
      ALOGE("transmit_Layer2Connection(): No valid Data in Transmit-Call");
      error  = ECD_ERROR_TRANSMIT;
   }
   return error;
}

/*===========================================================================
 *
 *  Function Name:  int open_vib_driver(const char *pathname, int hwLinkIndex)
 *
 ****************************************************************************
 *
 * @brief
 *
 ****************************************************************************
 *
 * @post
 *
 *===========================================================================*/
static int open_vib_driver(const char *pathname, int hwLinkIndex)
{
   int result = -1;

   //Memory-Mapping for the Desip-VIB-Module
   //Create dynamic VIBMemory-Buffer
   //Initialize Rx- and Tx-Mutex
   result = VIBMemoryMap(hwLinkIndex);
   if(result < 0)
   {
      return result;
   }

   //Initialize the Layer1-Module - Interface to HW-Driver-Module
   //The related fd will also be stored in static variable of this Module
   result = SCI_HW_Init(pathname, hwLinkIndex, connections[hwLinkIndex].uartSpeed);

   if(result < 0)
   {
      //Free the mapped VIBMemory-Buffer
      //Destroy the created Rx- and Tx-Mutex
      VIBMemoryUnMap(hwLinkIndex);
      return result;
   }
   else
   {
      //Opening a DESIP channel
      //Function Pointer to Callback-Function as parameter:
      //VIBGetChar(): Receive  single character from Buffer gRxBuf[]
      //VIBPutChar(): Transmit single character to Buffer   gTxBuf[]
      SIPOpen(VIBGetChar, VIBPutChar, hwLinkIndex);

      //Initialization of DeSIP-Rx/Tx-Buffer
      VIBInitVars(hwLinkIndex);
      return result;
   }
}

/*===========================================================================
 *
 *  Function Name:  void workerThreadDeSIPHandler(unsigned char *ptr, int hwLinkIndex)
 *
 ****************************************************************************
 *
 * @brief
 *
 ****************************************************************************
 *
 * @post
 *
 *===========================================================================*/
static void workerThreadDeSIPHandler(unsigned char *receive_buffer_ptr, int hwLinkIndex)
{
   char    *pBuf = (char *) receive_buffer_ptr;
   ROUTER_MESSAGE in_msg;
   int     DataPacketSize;

   SIP_Rx_FSM(hwLinkIndex);

   /*------------------------------------------------------------------------*/
   /*      Data-Packet-Receive-Loop                                          */
   /*      Copy the received Data-Packet into Receive-Buffer                 */
   /*      SIPRecv() returns the length of the received Data-Packet          */
   /*      If Return-Value is (-1) no further Packet is available            */
   /*------------------------------------------------------------------------*/
   while( (DataPacketSize = SIPRecv(pBuf, hwLinkIndex) ) != -1)
   {
      //New Data-Packet complete
      //Call the Router-API-Function to forward the HiSIP-Message
      in_msg.data_size = DataPacketSize;
      in_msg.data      = (char *) pBuf;
      if(HW_LINK_IDX0 == hwLinkIndex)
      {
         in_msg.event_id  = DESIP_EVENT_RECEIVE_DATA_PORT_0;
      }
      else if(HW_LINK_IDX1 == hwLinkIndex)
      {
         in_msg.event_id  = DESIP_EVENT_RECEIVE_DATA_PORT_1;
      }
      messageReceive(&in_msg);
   }

   // SIP_Rx_FSM and SIPRecv have now primed/setup global
   // variables, e.g. FLG_TXACK, used in below calls
   SIP_Tx_FSM(TX_FSM_EVENT_NORMAL_OP, 0x00, hwLinkIndex);
   VIBPutMsg(hwLinkIndex);
}

/*===========================================================================
 *
 *  Function Name:  initConnection
 *
 ****************************************************************************
 *
 * @brief           Initialisation of Data for new UART-Port-Connection
 *                  which is identified with parameter index
 *
 ****************************************************************************
 *
 * @return          bool
 *
 *===========================================================================*/
static bool initConnection(int index, ECD_RxTx_Connection *p_ConnectionState, unsigned int uartSpeed)
{
   bool result = true;
   connections[index].sem_start = 0;
   connections[index].pshared   = 0;
   connections[index].Connection_State.Tx_State_ptr  = p_ConnectionState->Tx_State_ptr;
   connections[index].Connection_State.Rx_State_ptr  = p_ConnectionState->Rx_State_ptr;
   connections[index].Connection_State.TxRx_path_ptr = p_ConnectionState->TxRx_path_ptr;

   // Mark send/transmit queue ID as unopened
   connections[index].QueueIDSend = -1;
   // Mark send/transmit queue ID as unopened
   connections[index].QueueIDReader = -1;
   // Store requested UART speed for later use
   connections[index].uartSpeed = uartSpeed;

   if(index == HW_LINK_IDX0)
   {
      const char *NameString = DESIP_MSG_QUEUE_Port0;
      strncpy(connections[index].QueueNameBuffer, NameString, sizeof(connections[index].QueueNameBuffer)-1);
      connections[index].OwnQueueName           = connections[index].QueueNameBuffer;
      connections[index].connectionIndex        = HW_LINK_IDX0;
      connections[index].p_ReaderThreadfunction = reader_thread_port0;
      connections[index].p_WorkerThreadfunction = worker_thread_port0;
   }
   else if(index == HW_LINK_IDX1)
   {
      const char *NameString = DESIP_MSG_QUEUE_Port1;
      strncpy(connections[index].QueueNameBuffer, NameString, sizeof(connections[index].QueueNameBuffer)-1);
      connections[index].OwnQueueName           = connections[index].QueueNameBuffer;
      connections[index].connectionIndex        = HW_LINK_IDX1;
      connections[index].p_ReaderThreadfunction = reader_thread_port1;
      connections[index].p_WorkerThreadfunction = worker_thread_port1;
   }
   else
   {
      connections[index].p_ReaderThreadfunction = NULL;
      connections[index].p_WorkerThreadfunction = NULL;
      connections[index].OwnQueueName           = NULL;
      ALOGE("initConnection(): UART-Connection %d  still not available",index);
      result = false;
   }

   return result;
}


/*===========================================================================
 *
 *  Function Name:  void *worker_thread_port0(void *para)
 *
 ****************************************************************************
 *
 * @brief           WorkerThread-Function for UART-Port 0
 *
 ****************************************************************************
 *
 * @post
 *
 *===========================================================================*/
static void *worker_thread_port0(void *para)
{
    Layer2Connection     *Connection_ptr       = (Layer2Connection *) para;
    ECD_RxTx_Connection  *Connection_State_ptr = &(Connection_ptr->Connection_State);
    const char           *DevicePath           = Connection_State_ptr->TxRx_path_ptr;
    char                  InputBuffer[MSG_INPUT_BUFFER_SIZE];
    ROUTER_MESSAGE        *in_msg_ptr;
    ssize_t               nBytesRead = 0;
    ssize_t               in_msg_expected;
    size_t                in_msg_len;
    int                   errvalue;
    char                  *in_Buffer_ptr;
    //Buffer to locate incoming Message-Data delivered by DeSIP-Function SIPRecv()
    unsigned char         ReceiveBuffer[512];
    bool                  isMessageReceived = false;
    int                   max_sd;
    timeval               timeout;
    fd_set                master_set, working_set;

    workerThreadRunning[HW_LINK_IDX0] = 1;

    /*------------------------------------------------------------------------*/
    /*   Check for right Connection-Environment                               */
    /*------------------------------------------------------------------------*/
    if(Connection_ptr != NULL)
    {
       if(Connection_ptr->connectionIndex != HW_LINK_IDX0)
       {
          ALOGE("worker_thread_port0(): Thread has started with corrupted Connection-Environment - Terminate thread");
          return NULL;
       }
    }
    else
    {
       ALOGE("worker_thread_port0(): Thread has started with corrupted Connection-Pointer - Terminate thread");
       return NULL;
    }

    /*------------------------------------------------------------------------*/
    /*   Open Desip-HW-Driver                                                 */
    /*------------------------------------------------------------------------*/
    int vib_fd = open_vib_driver(DevicePath, HW_LINK_IDX0);
    if(vib_fd < 0)
    {
       //Set the RX/TX-Connection-status if opening HW-Device failed
       *(Connection_State_ptr -> Tx_State_ptr) = ECD_CONNECTION_NO_CONNECTION;
       *(Connection_State_ptr -> Rx_State_ptr) = ECD_CONNECTION_NO_CONNECTION;
       workerThreadRunning[HW_LINK_IDX0] = 0;
       ALOGE("worker_thread_port0(): Unable to open Desip-HW-Driver for Port 0 - Terminate Worker-Thread-Loop");
    }
    else
    {
       //Set the RX/TX-Connection-status if opening HW-Device was successful
       *(Connection_State_ptr -> Tx_State_ptr) = ECD_CONNECTION_TX_CONNECTION;
       *(Connection_State_ptr -> Rx_State_ptr) = ECD_CONNECTION_RX_CONNECTION;
    }

    /*************************************************************/
    /* Initialize the master fd_set                              */
    /*************************************************************/
    FD_ZERO(&master_set);
    max_sd = Connection_ptr->QueueID;
    FD_SET(Connection_ptr->QueueID, &master_set);

    /*************************************************************/
    /* Initialize the timeval struct to 3 minutes.  If no        */
    /* activity after 3 minutes this program will end.           */
    /*************************************************************/
    timeout.tv_sec  = 3 * 60;
    timeout.tv_usec = 0;
    //Clear receive buffer
    memset(InputBuffer, 0, MSG_INPUT_BUFFER_SIZE);

    //Specify fix Parameter for receive-Call of Incoming-Messages
    in_msg_expected = (ssize_t)sizeof(ROUTER_MESSAGE);
    in_msg_len      = sizeof(InputBuffer);
    in_Buffer_ptr   = InputBuffer;
    in_msg_ptr      = (ROUTER_MESSAGE *) in_Buffer_ptr;

    /*------------------------------------------------------------------------*/
    /*   worker thread has completed the initialization                       */
    /*   Post the worker thread semaphore                                     */
    /*------------------------------------------------------------------------*/
    errno = EOK;
    if(sem_post(&Connection_ptr->worker_thread_sem) < 0)
    {
       errvalue = errno;
       ALOGE("worker_thread_port0(): Posting of worker_thread_sem returned with ErrorNumber = %d errno = %s",errvalue,strerror(errvalue));
       *(Connection_State_ptr -> Tx_State_ptr) = ECD_CONNECTION_NO_CONNECTION;
       *(Connection_State_ptr -> Rx_State_ptr) = ECD_CONNECTION_NO_CONNECTION;
       workerThreadRunning[HW_LINK_IDX0] = 0;
    }

    /*-----------------------------------------------------------------------------*/
    /*   Worker-Thread-Loop:                                                       */
    /*   Waiting for Incoming-Messages which wake up Worker-Thread                 */
    /*   Message can be send by Reader-Thread ==> DESIP_EVENT_RECEIVE_DATA_PORT_0  */
    /*   Message can be send by Application   ==> DESIP_EVENT_TRANSMIT_DATA_PORT_0 */
    /*-----------------------------------------------------------------------------*/
    while(workerThreadRunning[HW_LINK_IDX0])
    {
        if(messageReceived(&max_sd, Connection_ptr->QueueID, &master_set, &working_set, in_Buffer_ptr, in_msg_expected, timeout))
        {
           in_msg_ptr      = (ROUTER_MESSAGE *) in_Buffer_ptr;
           switch(in_msg_ptr->event_id)
           {
              case DESIP_EVENT_RECEIVE_DATA_PORT_0:
              {
                 //Incoming-Event came from Reader-Thread
                 ALOGV("worker_thread_port0(): Event from Reader-Thread received");
                 //Incoming-Data are already copied into DeSIP-Rx-Buffer
                 //Call directly the workerThreadDeSIPHandler()
                 //Returns if all available Receive-Data-Packets are routed via HiSIP-Router-API
                 workerThreadDeSIPHandler(ReceiveBuffer, HW_LINK_IDX0);
                 break;
              }
              case DESIP_EVENT_TRANSMIT_DATA_PORT_0:
              {
                 //Incoming-Event came from sending Application
                 ALOGV("worker_thread_port0(): Event from sending Application received");
                 //Copy Data from Transmit-Buffer into DeSIP-Packet-Buffer
                 if( (in_msg_ptr->data != NULL) && (in_msg_ptr->data_size > 0) )
                 {
                    //Call Desip-Function to transfer Transmit-Data into Transmit-State-Machine
                    if(SIPXmit((char *)in_msg_ptr->data, in_msg_ptr->data_size, HW_LINK_IDX0) == -1)
                    {
                       ALOGE("worker_thread_port0(): Unable to transfer data to Transmit-StateMachine");
                    }
                    else
                    {
                       //Call the workerThreadDeSIPHandler() to send the available Data
                       workerThreadDeSIPHandler(ReceiveBuffer, HW_LINK_IDX0);
                    }
                 }
                 else
                 {
                    ALOGE("worker_thread_port0(): Message is erroneously formatted");
                    ALOGE("worker_thread_port0(): in_msg_ptr->data_size: %d",(uint32_t)in_msg_ptr->data_size);
                 }

                 //Free the successful allocated data from transmit_Layer2Connection()
                 if(NULL != in_msg_ptr->data)
                 {
                    free(in_msg_ptr->data);
                    in_msg_ptr->data = NULL;
                 }
                 break;
              }
              default:
              {
                 ALOGE("worker_thread_port0(): mq_receive() delivered message with unexpected eventID = %d",in_msg_ptr->event_id);
                 break;
              }
           }
        }
        else
        {
           ALOGE("worker_thread_port0(): mq_receive() delivered message with unexpected size = %d", (int)nBytesRead);

           if(NULL != in_msg_ptr->data)
           {
              free(in_msg_ptr->data);
              in_msg_ptr->data = NULL;
           }
        }
    }
    if(Connection_ptr->QueueID >= 0)
    {
    	close(Connection_ptr->QueueID);
    }
    if(NULL != Connection_ptr->OwnQueueName)
    {
    	unlink(Connection_ptr->OwnQueueName);
    }
    return NULL;
}

/*===========================================================================
 *
 *  Function Name:  void *worker_thread_port1(void *para)
 *
 ****************************************************************************
 *
 * @brief           WorkerThread-Function for UART-Port 1
 *
 ****************************************************************************
 *
 * @post
 *
 *===========================================================================*/
static void *worker_thread_port1(void *para)
{
    Layer2Connection      *Connection_ptr       = (Layer2Connection *) para;
    ECD_RxTx_Connection   *Connection_State_ptr = &(Connection_ptr->Connection_State);
    const char            *DevicePath           = Connection_State_ptr->TxRx_path_ptr;
    char                  InputBuffer[MSG_INPUT_BUFFER_SIZE];
    ROUTER_MESSAGE        *in_msg_ptr;
    ssize_t               nBytesRead;
    ssize_t               in_msg_expected;
    size_t                in_msg_len;
    int                   errvalue;
    char                  *in_Buffer_ptr;
    //Buffer to locate incoming Message-Data delivered by DeSIP-Function SIPRecv()
    unsigned char         ReceiveBuffer[512];
    bool                  isMessageReceived = false;
    int                   max_sd;
    timeval               timeout;
    fd_set                master_set, working_set;

    workerThreadRunning[HW_LINK_IDX1] = 1;
    ALOGI("worker_thread_port1(): Entering...");

    /*------------------------------------------------------------------------*/
    /*   Check for right Connection-Environment                               */
    /*------------------------------------------------------------------------*/
    if(Connection_ptr != NULL)
    {
       if(Connection_ptr->connectionIndex != HW_LINK_IDX1)
       {
          ALOGE("worker_thread_port1(): Thread has started with corrupted Connection-Environment - Terminate thread");
          return NULL;
       }
    }
    else
    {
        ALOGE("worker_thread_port1(): Thread has started with corrupted Connection-Pointer - Terminate thread");
       return NULL;
    }

    /*------------------------------------------------------------------------*/
    /*   Open Desip-HW-Driver                                                 */
    /*------------------------------------------------------------------------*/
    if(workerThreadRunning[HW_LINK_IDX1])
    {
       int vib_fd = open_vib_driver(DevicePath, HW_LINK_IDX1);
       if(vib_fd < 0)
       {
          //Set the RX/TX-Connection-status if opening HW-Device failed
          *(Connection_State_ptr -> Tx_State_ptr) = ECD_CONNECTION_NO_CONNECTION;
          *(Connection_State_ptr -> Rx_State_ptr) = ECD_CONNECTION_NO_CONNECTION;
          workerThreadRunning[HW_LINK_IDX1] = 0;
          ALOGE("worker_thread_port0(): Unable to open Desip-HW-Driver for Port 0 - Terminate Worker-Thread-Loop");
       }
       else
       {
          //Set the RX/TX-Connection-status if opening HW-Device was successful
          *(Connection_State_ptr -> Tx_State_ptr) = ECD_CONNECTION_TX_CONNECTION;
          *(Connection_State_ptr -> Rx_State_ptr) = ECD_CONNECTION_RX_CONNECTION;
       }
    }


    //Clear receive buffer
    memset(InputBuffer, 0, MSG_INPUT_BUFFER_SIZE);

    //Specify fix Parameter for receive-Call of Incoming-Messages
    in_msg_expected = (ssize_t)sizeof(ROUTER_MESSAGE);
    in_msg_len      = sizeof(InputBuffer);
    in_Buffer_ptr   = InputBuffer;
    in_msg_ptr      = (ROUTER_MESSAGE *) in_Buffer_ptr;

    /*************************************************************/
    /* Initialize the master fd_set                              */
    /*************************************************************/
    FD_ZERO(&master_set);
    max_sd = Connection_ptr->QueueID;
    FD_SET(Connection_ptr->QueueID, &master_set);

    /*************************************************************/
    /* Initialize the timeval struct to 3 minutes.  If no        */
    /* activity after 3 minutes this program will end.           */
    /*************************************************************/
    timeout.tv_sec  = 3 * 60;
    timeout.tv_usec = 0;

    /*------------------------------------------------------------------------*/
    /*   worker thread has completed the initialization                       */
    /*   Post the worker thread semaphore                                     */
    /*------------------------------------------------------------------------*/
    errno = EOK;
    if(sem_post(&Connection_ptr->worker_thread_sem) < 0)
    {
       errvalue = errno;
       ALOGE("worker_thread_port1(): Posting of worker_thread_sem returned with ErrorNumber = %d errno = %s", errvalue,strerror(errvalue));
       *(Connection_State_ptr -> Tx_State_ptr) = ECD_CONNECTION_NO_CONNECTION;
       *(Connection_State_ptr -> Rx_State_ptr) = ECD_CONNECTION_NO_CONNECTION;
       workerThreadRunning[HW_LINK_IDX1] = 0;
    }

    /*-----------------------------------------------------------------------------*/
    /*   Worker-Thread-Loop:                                                       */
    /*   Waiting for Incoming-Messages which wake up Worker-Thread                 */
    /*   Message can be send by Reader-Thread ==> DESIP_EVENT_RECEIVE_DATA_PORT_1  */
    /*   Message can be send by Application   ==> DESIP_EVENT_TRANSMIT_DATA_PORT_1 */
    /*-----------------------------------------------------------------------------*/
    while(workerThreadRunning[HW_LINK_IDX1])
    {
        if(messageReceived(&max_sd, Connection_ptr->QueueID, &master_set, &working_set, in_Buffer_ptr, in_msg_expected, timeout))
        {
           in_msg_ptr      = (ROUTER_MESSAGE *) in_Buffer_ptr;
           switch(in_msg_ptr->event_id)
           {
              case DESIP_EVENT_RECEIVE_DATA_PORT_1:
              {
                 //Incoming-Event came from Reader-Thread
                 ALOGV("worker_thread_port1(): Event from Reader-Thread received");
                 //Incoming-Data are already copied into DeSIP-Rx-Buffer
                 //Call directly the workerThreadDeSIPHandler()
                 //Returns if all available Receive-Data-Packets are routed via HiSIP-Router-API
                 workerThreadDeSIPHandler(ReceiveBuffer, HW_LINK_IDX1);
                 break;
              }
              case DESIP_EVENT_TRANSMIT_DATA_PORT_1:
              {
                 //Incoming-Event came from sending Application
                 ALOGV("worker_thread_port1(): Event from sending Application received");

                 //Copy Data from Transmit-Buffer into DeSIP-Packet-Buffer
                 if( (in_msg_ptr->data != NULL) && (in_msg_ptr->data_size > 0) )
                 {
                    //Call Desip-Function to transfer Transmit-Data into Transmit-State-Machine
                    if(SIPXmit((char *)in_msg_ptr->data, in_msg_ptr->data_size, HW_LINK_IDX1) == -1)
                    {
                       ALOGE("worker_thread_port1(): Unable to transfer data to Transmit-StateMachine");
                    }
                    else
                    {
                       //Call the workerThreadDeSIPHandler() to send the available Data
                       workerThreadDeSIPHandler(ReceiveBuffer, HW_LINK_IDX1);
                    }
                 }
                 else
                 {
                    ALOGE("worker_thread_port1(): Message is erroneously formatted");
                    ALOGE("worker_thread_port1(): in_msg_ptr->data_size: %d", (uint32_t)in_msg_ptr->data_size);
                 }

                 //Free the successful allocated data from transmit_Layer2Connection()
                 if(NULL != in_msg_ptr->data)
                 {
                    free(in_msg_ptr->data);
                    in_msg_ptr->data = NULL;
                 }
                 break;
              }
              default:
              {
                 ALOGE("worker_thread_port1(): read() delivered message with unexpected eventID = %d",in_msg_ptr->event_id);
                 break;
              }
           }
        }
    }
    ALOGE("worker_thread_port1(): exiting");
    return NULL;
}

/*===========================================================================
 *
 *  Function Name:  void *reader_thread_port0(void *para)
 *
 ****************************************************************************
 *
 * @brief
 *
 ****************************************************************************
 *
 * @post
 *
 *===========================================================================*/
static void *reader_thread_port0(void *para)
{
    Layer2Connection     *Connection_ptr = (Layer2Connection *) para;
    const char           *QueueName      = DESIP_MSG_QUEUE_Port0;
    int                  nbytesRead      = 0;
    int                  errvalue;
    ROUTER_MESSAGE       out_msg;
    int               msg_len         = sizeof(out_msg);

    readerThreadRunning[HW_LINK_IDX0] = 1;

    //Fill the Message-Structure for Reader-Thread-Event
    out_msg.event_id                     = DESIP_EVENT_RECEIVE_DATA_PORT_0;
    out_msg.data_size                    = 0;
    out_msg.data                         = NULL;

    /*------------------------------------------------------------------------*/
    /*   Check for right connection                                           */
    /*------------------------------------------------------------------------*/
    if(Connection_ptr->connectionIndex != HW_LINK_IDX0)
    {
       ALOGE("worker_thread_port0(): Mismatch in Connection-Data");
    }

    /*------------------------------------------------------------------------*/
    /*   Open Port0-MessageQueue                                              */
    /*------------------------------------------------------------------------*/
    if(readerThreadRunning[HW_LINK_IDX0])
    {
       errno = EOK;
       if ( (Connection_ptr->QueueIDReader = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
       {
    	   ALOGE("socket error");
    	   return NULL;
       }


	   struct sockaddr_un addr;
	   memset(&addr, 0, sizeof(addr));
	   addr.sun_family = AF_UNIX;
           strncpy(addr.sun_path, QueueName, sizeof(addr.sun_path)-1);

	   int cntr = 0;

	   while ((connect(Connection_ptr->QueueIDReader, (struct sockaddr*)&addr, sizeof(addr)) == -1) && cntr < 10)
	   {
		   errvalue = errno;
	      ALOGE("reader_thread_port0 connect socket error %d %s Cntr = %d",errvalue,QueueName,cntr);
	      cntr++;
	      sleep(1);
	   }

       if(Connection_ptr->QueueIDReader < 0)
       {
          errvalue = errno;
          ALOGE("reader_thread_port0(): Open Message-Queue returned with ErrorNumber = %d errno = %s",errvalue,strerror(errvalue));
          readerThreadRunning[HW_LINK_IDX0] = 0;
       }
    }

    /*------------------------------------------------------------------------*/
    /*   reader thread has completed its initialization                       */
    /*   Post the reader thread semaphore                                     */
    /*------------------------------------------------------------------------*/
    errno = EOK;
    if(sem_post(&Connection_ptr->reader_thread_sem) < 0)
    {
       errvalue = errno;
       ALOGE("reader_thread_port0(): Posting of reader_thread_sem returned with ErrorNumber = %d error = %s",errvalue,strerror(errvalue));
       readerThreadRunning[HW_LINK_IDX0] = 0;
    }

    /*------------------------------------------------------------------------*/
    /*   Reader-Thread-Loop:                                                  */
    /*   - Terminal is configured in Raw-Binary-Mode with VMIN = 1            */
    /*   - Read-Call waits for at least 1 incoming Data-Byte                  */
    /*   - If more incoming data are available they will be read at once      */
    /*   - After Read-Call received Bytes are present in Buffer "serbuf"      */
    /*   - Then Data will be copied into DeSIP-Rx-Buffer for processing       */
    /*   - To process the data Context-Switch to Worker-Thread is triggered   */
    /*     via sending event to DeSIP-Worker-Queue                            */
    /*   - Then Thread goes to sleep until wakeup from Terminal-Signal        */
    /*------------------------------------------------------------------------*/
    while(readerThreadRunning[HW_LINK_IDX0])
    {
        //Read-Access to the DESIP-DeviceDriver
        if(VIBReadDataFromDevice(&nbytesRead, HW_LINK_IDX0))
        {
            //Trigger Context-Switch to Worker-Thread - No Data-Transfer
            //Data are already available in DeSIP-Rx-Buffer
            errno = EOK;
            if (write(Connection_ptr->QueueIDReader, &out_msg, msg_len) != msg_len)
            {
                errvalue = errno;
                ALOGE("reader_thread_port0(): mq_send() returned with ErrorNumber = %d",errvalue);
                ALOGE("Meaning of this errno: %s", strerror(errvalue));
            }
        }
    }
    if(Connection_ptr->QueueIDReader >= 0)
    {
       close(Connection_ptr->QueueIDReader);
    }
    return NULL;
}

/*===========================================================================
 *
 *  Function Name:  void *reader_thread_port1(void *para)
 *
 ****************************************************************************
 *
 * @brief
 *
 ****************************************************************************
 *
 * @post
 *
 *===========================================================================*/
static void *reader_thread_port1(void *para)
{
    Layer2Connection     *Connection_ptr = (Layer2Connection *) para;
    const char           *QueueName      =  DESIP_MSG_QUEUE_Port1;
    int                  nbytesRead      =  0;
    int                  errvalue;
    ROUTER_MESSAGE       out_msg;
    int               msg_len         = sizeof(out_msg);

    readerThreadRunning[HW_LINK_IDX1] = 1;

    //Fill the Message-Structure for Reader-Thread-Event
    out_msg.event_id                     = DESIP_EVENT_RECEIVE_DATA_PORT_1;
    out_msg.data_size                    = 0;
    out_msg.data                         = NULL;

    /*------------------------------------------------------------------------*/
    /*   Check for right connection                                           */
    /*------------------------------------------------------------------------*/
    if(Connection_ptr->connectionIndex != HW_LINK_IDX1)
    {
       ALOGE("worker_thread_port1(): Mismatch in Connection-Data");
    }

    /*------------------------------------------------------------------------*/
    /*   Open Port1-MessageQueue                                              */
    /*------------------------------------------------------------------------*/
    if(readerThreadRunning[HW_LINK_IDX1])
    {
      errno = EOK;
      if ( (Connection_ptr->QueueIDReader = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
      {
         ALOGE("reader_thread_port1() socket(): socket error");
         return NULL;
      }

      struct sockaddr_un addr;
      memset(&addr, 0, sizeof(addr));
      addr.sun_family = AF_UNIX;
      strncpy(addr.sun_path, QueueName, sizeof(addr.sun_path)-1);

      if (connect(Connection_ptr->QueueIDReader, (struct sockaddr*)&addr, sizeof(addr)) == -1)
      {
        ALOGE("reader_thread_port1 connect(): socket error");
        return NULL;
      }

       if(Connection_ptr->QueueIDReader < 0)
       {
          errvalue = errno;
          ALOGE("reader_thread_port1(): Open Message-Queue returned with ErrorNumber = %d errno = %s",errvalue,strerror(errvalue));
          readerThreadRunning[HW_LINK_IDX1] = 0;
       }
    }

    /*------------------------------------------------------------------------*/
    /*   reader thread has completed its initialization                       */
    /*   Post the reader thread semaphore                                     */
    /*------------------------------------------------------------------------*/
    errno = EOK;
    if(sem_post(&Connection_ptr->reader_thread_sem) < 0)
    {
       errvalue = errno;
       ALOGE("reader_thread_port1(): Posting of reader_thread_sem returned with ErrorNumber = %d errno = %s ",errvalue,strerror(errvalue));
       readerThreadRunning[HW_LINK_IDX1] = 0;
    }

    /*------------------------------------------------------------------------*/
    /*   Reader-Thread-Loop:                                                  */
    /*   - Terminal is configured in Raw-Binary-Mode with VMIN = 1            */
    /*   - Read-Call waits for at least 1 incoming Data-Byte                  */
    /*   - If more incoming data are available they will be read at once      */
    /*   - After Read-Call received Bytes are present in Buffer "serbuf"      */
    /*   - Then Data will be copied into DeSIP-Rx-Buffer for processing       */
    /*   - To process the data Context-Switch to Worker-Thread is triggered   */
    /*     via sending event to DeSIP-Worker-Queue                            */
    /*   - Then Thread goes to sleep until wakeup from Terminal-Signal        */
    /*------------------------------------------------------------------------*/
    while(readerThreadRunning[HW_LINK_IDX1])
    {
        //Read-Access to the DESIP-DeviceDriver
        if(VIBReadDataFromDevice(&nbytesRead, HW_LINK_IDX1))
        {
            //Trigger Context-Switch to Worker-Thread - No Data-Transfer
            //Data are already available in DeSIP-Rx-Buffer
            errno = EOK;
            if (write(Connection_ptr->QueueIDReader, &out_msg, msg_len) != msg_len)
            {
                errvalue = errno;
                ALOGE("reader_thread_port1(): socket returned with ErrorNumber = %d Meaning of this errno: %s",errvalue,strerror(errvalue));
            }
        }
    }
    if(Connection_ptr->QueueIDReader >= 0)
    {
       close(Connection_ptr->QueueIDReader);
    }
    return NULL;
}

inline static bool messageReceived(int *max_sd,
                            int listen_sd,
                            fd_set *master_set,
                            fd_set *working_set,
                            char   *message,
                            ssize_t expectedMessageLenght,
                            timeval timeout)
{
    bool    isMessageReceived = false;
    int     close_conn = false;
    ssize_t nBytesRead = 0;
    int     rc = 0;

    do
    {
        /**********************************************************/
        /* Copy the master fd_set over to the working fd_set.     */
        /**********************************************************/
        memcpy(working_set, master_set, sizeof(*master_set));

        /**********************************************************/
        /* Call select() and wait 5 minutes for it to complete.   */
        /**********************************************************/
        ALOGV("Waiting on select()...");
        rc = select(*max_sd + 1, working_set, NULL, NULL, &timeout);

        /**********************************************************/
        /* Check to see if the select call failed.                */
        /**********************************************************/
        if (rc < 0)
        {
            ALOGE("  select() failed");
            break;
        }

        /**********************************************************/
        /* Check to see if the 5 minute time out expired.         */
        /**********************************************************/
        if (rc == 0)
        {
            ALOGV("  select() timed out.");
            break;
        }

        /**********************************************************/
        /* One or more descriptors are readable.  Need to         */
        /* determine which ones they are.                         */
        /**********************************************************/
        int desc_ready = rc;

        for (int i=0; i <= *max_sd  &&  desc_ready > 0  && !isMessageReceived; ++i)
        {
            /*******************************************************/
            /* Check to see if this descriptor is ready            */
            /*******************************************************/
            if (FD_ISSET(i, working_set))
            {
                /****************************************************/
                /* A descriptor was found that was readable - one   */
                /* less has to be looked for.  This is being done   */
                /* so that we can stop looking at the working set   */
                /* once we have found all of the descriptors that   */
                /* were ready.                                      */
                /****************************************************/
                desc_ready -= 1;

                /****************************************************/
                /* Check to see if this is the listening socket     */
                /****************************************************/
                if (i == listen_sd)
                {
                     int new_sd = 0;
                     ALOGV("  New incoming connection");
                    /*************************************************/
                    /* Accept all incoming connections that are      */
                    /* queued up on the listening socket before we   */
                    /* loop back and call select again.              */
                    /*************************************************/
                    do
                    {
                      ALOGV("accept ... id == %d", listen_sd);
                      /**********************************************/
                      /* Accept each incoming connection.  If       */
                      /* accept fails with EWOULDBLOCK, then we     */
                      /* have accepted all of them.  Any other      */
                      /* failure on accept will cause us to end the */
                      /* server.                                    */
                      /**********************************************/
                      new_sd = accept(listen_sd, NULL, NULL);
                      if (new_sd < 0)
                      {
                         if (errno != EWOULDBLOCK)
                         {
                            ALOGE("  accept() failed");
                         }
                         break;
                      }
                      ALOGV("Done! accepted ... id == %d",listen_sd);
                      /**********************************************/
                      /* Add the new incoming connection to the     */
                      /* master read set                            */
                      /**********************************************/

                      FD_SET(new_sd, master_set);
                      if (new_sd > *max_sd) {
                         *max_sd = new_sd;
                          ALOGI("New connection max_sd id = %d", *max_sd);
                      }
                      /**********************************************/
                      /* Loop back up and accept another incoming   */
                      /* connection                                 */
                      /**********************************************/
                    } while (new_sd != -1);
                }

                /****************************************************/
                /* This is not the listening socket, therefore an   */
                /* existing connection must be readable             */
                /****************************************************/
                else
                {
                   ALOGV("  Descriptor %d is readable", i);
                   close_conn = false;
                   /*************************************************/
                   /* Receive all incoming data on this socket      */
                   /* before we loop back and call select again.    */
                   /*************************************************/
                   do
                   {
                        /**********************************************/
                        /* Receive data on this connection until the  */
                        /* recv fails with EWOULDBLOCK.  If any other */
                        /* failure occurs, we will close the          */
                        /* connection.                                */
                        /**********************************************/
                        nBytesRead = recv(i, message, expectedMessageLenght, 0);
                        if (nBytesRead < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                ALOGE("  recv() failed") ;
                                close_conn = true;
                            }
                            break;
                        }

                        /**********************************************/
                        /* Check to see if the connection has been    */
                        /* closed by the client                       */
                        /**********************************************/
                        if (nBytesRead == 0)
                        {
                             ALOGE("  Connection closed ");
                             close_conn = true;
                             break;
                        }

                        /**********************************************/
                        /* Data was received                          */
                        /**********************************************/
                        // Have expcected data been received?
                        isMessageReceived = (nBytesRead == expectedMessageLenght);

                   } while (!isMessageReceived);


                   /*************************************************/
                   /* If the close_conn flag was turned on, we need */
                   /* to clean up this active connection.  This     */
                   /* clean up process includes removing the        */
                   /* descriptor from the master set and            */
                   /* determining the new maximum descriptor value  */
                   /* based on the bits that are still turned on in */
                   /* the master set.                               */
                   /*************************************************/
                   if (close_conn)
                   {
                      ALOGI("Close connection id = %d",i);
                      close(i);
                      FD_CLR(i, master_set);
                      if (i == *max_sd)
                      {
                         while (FD_ISSET(*max_sd, master_set) == false)
                            *max_sd -= 1;
                      }
                   }
                } /* End of existing connection is readable */
            } /* End of if (FD_ISSET(i, working_set)) */
        } /* End of loop through selectable descriptors */
    } while(!isMessageReceived);
    return isMessageReceived;
}

