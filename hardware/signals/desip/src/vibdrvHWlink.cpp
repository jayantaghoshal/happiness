/***********************************************************************
*       (c) '2016' Delphi Delco Electronics Systems
*            All rights Reserved.  Delphi D confidential
***********************************************************************/
/***********************************************************************
*
*  Title:              vibdrvHWlink.cpp
*
*  Description:        VIB datalink layer
*
*  Created:            12-02-16
*
*  Author:             Dieter Slowik
*
*  Configuration ID:
*
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <linux/sched.h>
#include <termios.h>

#include "vibdrvHWlink.h"
#include "desip.h"

#undef LOG_TAG
#define LOG_TAG "DESIP"
#include <cutils/log.h>


//Global defines ---------------------------------------------------------------------------------------
#define TRACE_OVERRUN
#define RX_BUF_SIZE         8192
#define TX_BUF_SIZE         8192
#define TX_ISR_BUF_SIZE     512
#define SERIAL_BUF_SIZE     4096  //4 KByte-Buffer for serial Device

typedef struct
{
    volatile char           *gRxBuf;                 //Rx buffer
    int                     gRxBufOutIndex;          //index to the next character to be extracted from gRxBuf
    int                     gRxBufInIndex;           //index to the next character to be inserted to gRxBuf
    volatile char           *gTxBuf;                 //Tx buffer
    unsigned int            gTxBufInIndex;           //index to transmit buffer incoming characters
    unsigned int            gTxBufOutIndex;          //index to transmit buffer outgoing characters
    volatile unsigned char  *gTxISRBuf;              //Tx ISR buffer
    volatile unsigned int   gRxErrorCount;           //Rx ErrorCount
    pthread_mutex_t         MutexTxIndex;            //mutex for tx buffer
    pthread_mutex_t         MutexRxIndex;            //mutex for rx buffer
    char                    serbuf[SERIAL_BUF_SIZE]; //static Buffer for UART-Communication = SERIAL_BUF_SIZE KByte
    int                     ser_fd;                  //static file descriptors for UART ports
}HWLink;

//Global variables in this Module ----------------------------------------------------------------------
HWLink hwLinks[NUM_HW_LINKS];

//=============================================================================
//   Function-Prototypes
//=============================================================================
static int doRxInterrupt(int nbr, int index);  //Has nothing to do with HW-Interrupt-Handler
                                               //Means interrupt inside the DeSIP-Handshake

/**********************************************************************************
 *  Function Name: VIBMemoryMap
 *
 *  Description:
 *  This function provides memory mapping for the VIB driver.
 *  This function should only be called once per battery connect
 *
 **********************************************************************************/
int VIBMemoryMap(int index)
{
    /*map tx buffer */
    if ( (hwLinks[index].gTxBuf = (char *)malloc(TX_BUF_SIZE)) == NULL)
    {
        ALOGE("Mapping of gTxBuf[%d] failed: errno = %s",index,strerror(errno));
        return -1;
    }
    /*map txi buffer */
    if ( (hwLinks[index].gTxISRBuf = (unsigned char *)malloc(TX_ISR_BUF_SIZE)) == NULL)
    {
        ALOGE("Mapping of gTxISRBuf[%d] failed: errno = %s",index, strerror(errno));
        return -1;
    }
    /*map rx buffer */
    if ( (hwLinks[index].gRxBuf = (char *)malloc(RX_BUF_SIZE)) == NULL)
    {
        ALOGE("Mapping of gRxBuf[%d] failed: errno = %s", index  ,strerror(errno));
        return -1;
    }

    /*initialize tx buffer mutex and char tx thread */
    if (EOK != pthread_mutex_init(&hwLinks[index].MutexTxIndex,NULL))
    {
        ALOGE("ERROR, pthread_mutex_init(&hwLinks[%d].MutexTxIndex,NULL) failed",index);
        return -1;
    }

    /*initialize rx buffer mutex and char rx thread */
    if (EOK != pthread_mutex_init(&hwLinks[index].MutexRxIndex,NULL))
    {
        ALOGE("ERROR, pthread_mutex_init(&hwLinks[%d].MutexRxIndex,NULL) failed",index);
        return -1;
    }

    return 0;
}

/**********************************************************************************
 *  Function Name: VIBMemoryUnMap
 *
 *  Description:
 *    This function unmappes the memory for the VIB driver.
 **********************************************************************************/
int VIBMemoryUnMap(int index)
{
    /*unmap tx buffer */
    if (nullptr != hwLinks[index].gTxBuf)
    {
        free((void*) hwLinks[index].gTxBuf);
        hwLinks[index].gTxBuf = nullptr;
    }

    /*unmap txi buffer */
    if (nullptr != hwLinks[index].gTxISRBuf)
    {
        free((void*) hwLinks[index].gTxISRBuf);
        hwLinks[index].gTxISRBuf = nullptr;
    }

    /*unmap rx buffer */
    if (nullptr != hwLinks[index].gRxBuf)
    {
        free((void*) hwLinks[index].gRxBuf);
        hwLinks[index].gRxBuf = nullptr;
    }

    /*destroy tx buffer mutex */
    if (EOK != pthread_mutex_destroy(&hwLinks[index].MutexTxIndex))
    {
        ALOGE("ERROR, pthread_mutex_destroy(&hwLinks[%d].MutexTxIndex) failed", index);
        return -1;
    }

    /*destroy rx buffer mutex */
    if (EOK != pthread_mutex_destroy(&hwLinks[index].MutexRxIndex))
    {
        ALOGE("ERROR, pthread_mutex_destroy(&hwLinks[%d].MutexRxIndex) failed", index);
        return -1;
    }

    return 0;
}

/**********************************************************************************
 *  Function Name: doRxInterrupt
 *
 *  Description:
 *
 *
 **********************************************************************************/
static int doRxInterrupt(int nbr, int index)
{
    int i, j;
    int tmp2;

    tmp2 = nbr;
    pthread_mutex_lock(&hwLinks[index].MutexRxIndex);

    /* read appropriate number of bytes */
    if ((hwLinks[index].gRxBufInIndex + tmp2) >= RX_BUF_SIZE)
    {
        tmp2 = RX_BUF_SIZE - hwLinks[index].gRxBufInIndex;
        for (i = 0; i<tmp2; i++)
        {
            hwLinks[index].gRxBuf[ hwLinks[index].gRxBufInIndex + i ] = hwLinks[index].serbuf[i];
        }
        hwLinks[index].gRxBufInIndex += tmp2;
        hwLinks[index].gRxBufInIndex %= RX_BUF_SIZE;
        nbr -= tmp2;
        for (j=0; j<nbr; i++, j++)
        {
            hwLinks[index].gRxBuf[ hwLinks[index].gRxBufInIndex + j ] = hwLinks[index].serbuf[i];
        }
        hwLinks[index].gRxBufInIndex += nbr;
        hwLinks[index].gRxBufInIndex %= RX_BUF_SIZE;
    }
    else
    {
        for (i = 0; i < tmp2; i++)
        {
            hwLinks[index].gRxBuf[ hwLinks[index].gRxBufInIndex + i ] = hwLinks[index].serbuf[i];
        }
        hwLinks[index].gRxBufInIndex += tmp2;
        hwLinks[index].gRxBufInIndex %= RX_BUF_SIZE;
    }
    pthread_mutex_unlock(&hwLinks[index].MutexRxIndex);
    return 1;
}

/**********************************************************************************
 *  Function Name: SCI_HW_Init
 *
 *  Description:
 *  This function initializes the H/W layer
 **********************************************************************************/
int SCI_HW_Init(const char *pathname, int index, unsigned int uartSpeed)
{
    struct termios termiConfig;

    //Open Terminal-Port:
    //In Blocking-Mode:   Default-Mode ==> Don't set flag O_NONBLOCK
    //In Read-Write-Mode: Set flag O_RDWR
    hwLinks[index].ser_fd = open(pathname, O_RDWR | O_SYNC);
    if(hwLinks[index].ser_fd < 0)
    {
        ALOGE("SCI_HW_Init(): Unable to open UART-Device for port %d with path = %s", index,pathname);
    }
    else
    {
       // Get the current configuration of the serial interface
       if(tcgetattr(hwLinks[index].ser_fd, &termiConfig) < 0)
       {
           ALOGE("SCI_HW_Init(): Unable to get current configuration for port %d with path = %s", index,pathname);
           close(hwLinks[index].ser_fd);
           hwLinks[index].ser_fd = -1;
       }
       else
       {
           //Set Communication speed via predefines
           if(cfsetspeed(&termiConfig, (speed_t)uartSpeed) < 0)
           {
               ALOGE("SCI_HW_Init(): Unable to set Bitrate = B115200 on for port %d with path = %s", index,pathname);
               close(hwLinks[index].ser_fd);
               hwLinks[index].ser_fd = -1;
           }
           else
           {
               //Set the terminal port to "RAW-Binary-Mode" - no input/output processing
               //Implicit set of non-canonical mode: termios.c_lflag &= ~ICANON
               //Implicit set of VMIN = 1, Terminal-Read waits for at least 1 character
               cfmakeraw(&termiConfig);

               //Apply the new configuration immediately with parameter TCSANOW
               if(tcsetattr(hwLinks[index].ser_fd, TCSANOW, &termiConfig) < 0)
               {
                   ALOGE("SCI_HW_Init(): Unable to set new configuration on port %d with path = %s", index,pathname);
                   close(hwLinks[index].ser_fd);
                   hwLinks[index].ser_fd = -1;
               }
               else
               {
                   ALOGI("UART-Device for port %d is open and ready to work: fd = %d",index, hwLinks[index].ser_fd);
               }
           }
       }
    }

    return hwLinks[index].ser_fd;
}

/*********************************************************************************
 *
 *  Function name: VIBPutChar
 *
 *  Description:
 *    This function is called periodically by the desip FSM (SIPFSM)
 *    Its purpose is to send outgoing characters to the physical H/W.  Because of
 *      the lack of H/W buffer in the SH4 SCI, a S/W buffer gTxDMABuf is
 *      implemented.
 *    This function simply push the character into gTxDMABuf for pthread_DMATx to
 *      send.
 *    Optimally, the function should be made inline because of the frequence of
 *      the calls.
 *    The section enclosed within #ifdef NO_SCI will be removed in production code
 *********************************************************************************/
int VIBPutChar(int c, int index)
{
    unsigned char d;
    int ret;

    d = (unsigned char) (c & 0xff);

    pthread_mutex_lock(&hwLinks[index].MutexTxIndex);

    /*------- if buffer is not full -------------------------*/
    if( ((hwLinks[index].gTxBufInIndex + 1) % TX_BUF_SIZE) != hwLinks[index].gTxBufOutIndex)
    {
        hwLinks[index].gTxBuf[hwLinks[index].gTxBufInIndex++] = d;
        hwLinks[index].gTxBufInIndex %= TX_BUF_SIZE;
        ret = c;
    }
    else
    {
        /*------- buffer is full, return -1 -------------------*/
        ALOGW("VIBPutChar failed, buffer might be full");
        ret = -1;
    }
    pthread_mutex_unlock(&hwLinks[index].MutexTxIndex);
    return(ret);
}

/*********************************************************************************
 *
 *  Function name: VIBGetChar
 *
 *  Description:
 *    This function is called periodically by the desip FSM (SIPFSM).  Its
 *      purpose is to retrieve incoming characters from the physical H/W.
 *    Optimally, the function should be made inline because of the frequence of
 *      the calls.
 *    The section enclosed within #ifdef NO_SCI will be removed in production code
 *********************************************************************************/
int VIBGetChar(int index)
{
    int c, retval;

    pthread_mutex_lock(&hwLinks[index].MutexRxIndex);

    /*if there is a new character, extract it, else return -1 */
    if ( hwLinks[index].gRxBufOutIndex !=  hwLinks[index].gRxBufInIndex )
    {
        c = hwLinks[index].gRxBuf[ hwLinks[index].gRxBufOutIndex ];    /*extract character indexed by gRxDMABufOutIndex */
        hwLinks[index].gRxBufOutIndex = (hwLinks[index].gRxBufOutIndex + 1) % RX_BUF_SIZE;
        retval = c & 0xff;
    }
    else
    {
        retval = -1;
    }

    pthread_mutex_unlock(&hwLinks[index].MutexRxIndex);
    return(retval);
}

/*********************************************************************************
 *
 *  Function name: VIBPutMsg
 *
 *  Description:
 *********************************************************************************/
void VIBPutMsg(int index)
{
    int           len;
    int           nBytes;
    int           nWrite;
    unsigned char *sendptr;
    int           writeErrorCounter = 0;

    pthread_mutex_lock(&hwLinks[index].MutexTxIndex);

    // Initiate transmit if there are new characters, and SCIF is available
    if( hwLinks[index].gTxBufInIndex != hwLinks[index].gTxBufOutIndex )
    {
        if( hwLinks[index].gTxBufInIndex > hwLinks[index].gTxBufOutIndex)
        {
            len = hwLinks[index].gTxBufInIndex - hwLinks[index].gTxBufOutIndex;
            memcpy((char*) hwLinks[index].gTxISRBuf, (char*) (&hwLinks[index].gTxBuf[ hwLinks[index].gTxBufOutIndex ]), len);
        }
        else
        {
            // The buffer's in-index has wrapped
            int endOfBufLen   = TX_BUF_SIZE - hwLinks[index].gTxBufOutIndex;  // Bytes before wrapping (end of buffer)
            int startOfBufLen = hwLinks[index].gTxBufInIndex;                 // Bytes after wrapping (start of buffer)
            len               = endOfBufLen + startOfBufLen;

            memcpy((char*) hwLinks[index].gTxISRBuf, (char*) (&hwLinks[index].gTxBuf[ hwLinks[index].gTxBufOutIndex ]), endOfBufLen);
            memcpy((char*) hwLinks[index].gTxISRBuf + endOfBufLen, (char*) hwLinks[index].gTxBuf, startOfBufLen);
        }

        // Send-Loop
        nWrite  = len;
        sendptr = (unsigned char *) hwLinks[index].gTxISRBuf;
        while(nWrite > 0)
        {
           // Driver-Call to send Data from gTxISRBuf[] to seriel Device
           nBytes = write(hwLinks[index].ser_fd, sendptr, nWrite);
           if(nBytes > 0)
           {
              nWrite  = nWrite - nBytes;
              sendptr = sendptr + nBytes;

              // Update the Position-Index of Desip-Transmit-Buffer
              hwLinks[index].gTxBufOutIndex += nBytes;

              // Modulo-Check of Position-Index for the Ring-Buffer
              hwLinks[index].gTxBufOutIndex %= TX_BUF_SIZE;
           }
           else if(nBytes < 0)
           {
              writeErrorCounter++;
              ALOGE("VIBPutMsg(): Terminal-Write() on port %d returned with error", index);
              if(10 == writeErrorCounter)
              {
                 // We do not want to while forever
                 break;
              }
           }
        }
        ALOGV("VIBPutMsg(): %d bytes successfully send to terminal port %d", (len - nWrite), index);
    }
    pthread_mutex_unlock(&hwLinks[index].MutexTxIndex);
}

/**********************************************************************************
 *  Function Name: VIBReadDataFromDevice
 *
 *  Description:
 *
 *
 **********************************************************************************/
bool VIBReadDataFromDevice(int *nbytesRead, int index)
{
    fd_set         rfds;
    struct timeval tv;
    int            retval;

    // Watch UART to see when it has input
    FD_ZERO(&rfds);
    FD_SET(hwLinks[index].ser_fd, &rfds);

    // Wait for up to 10 seconds
    tv.tv_sec  = 10;
    tv.tv_usec = 0;

    retval = select(hwLinks[index].ser_fd + 1, &rfds, NULL, NULL, &tv);
    if (retval == -1)
    {
        ALOGE ("select call for serial port %d failed with errno = %d", index,errno);
    }
    else if(retval)
    {
        //-----------------------------------------------------------------
        // Driver-Call to read Data from seriel Device/UART to serbuf[]
        //------------------------------------------------------------------
        *nbytesRead = read(hwLinks[index].ser_fd, &hwLinks[index].serbuf, sizeof(hwLinks[index].serbuf));
        if(*nbytesRead < 0)
        {
            ALOGE("Read() to serial port %d returned with Error", index);
        }
        else
        {
            ALOGV("Serial terminal port %d received %d bytes", index, *nbytesRead);

            // Copy new data from HW-Device-Buffer serbuf[] into DeSIP-Rx-Buffer
            doRxInterrupt(*nbytesRead, index);
            return true;
        }
    }
    else
    {
        // Every 10:th second you will end up here if no data is arriving
        ALOGV("No data to read for port %d", index);
    }

    return false;
}

/**********************************************************************************
 *  Function Name: VIBInitVars
 *
 *  Description:
 *
 *
 **********************************************************************************/
void VIBInitVars(int index)
{
    pthread_mutex_lock(&hwLinks[index].MutexRxIndex);
    hwLinks[index].gRxBufOutIndex = 0;
    hwLinks[index].gRxBufInIndex  = 0;
    pthread_mutex_unlock(&hwLinks[index].MutexRxIndex);
    pthread_mutex_lock(&hwLinks[index].MutexTxIndex);
    hwLinks[index].gTxBufInIndex  = 0;
    hwLinks[index].gTxBufOutIndex = 0;
    pthread_mutex_unlock(&hwLinks[index].MutexTxIndex);
    hwLinks[index].gRxErrorCount  = 0;
}

/**********************************************************************************
 *  Function Name: VIBGetRxErrorCount
 *
 *  Description:
 *
 *
 **********************************************************************************/
unsigned int VIBGetRxErrorCount(int index)
{
    return hwLinks[index].gRxErrorCount;
}
