/* Copyright 2005 Delphi Delco Electronics */

#ifndef VIB_DRV_HW_LINK_H
#define VIB_DRV_HW_LINK_H

#ifndef EOK
#define EOK 0
#endif

#define NUM_HW_LINKS                 (3)
#define HW_LINK_IDX0                 (0)
#define HW_LINK_IDX1                 (1)
#define HW_LINK_IDX2                 (2)

int           SCI_HW_Init(const char *pathname, int index, unsigned int uartSpeed);
int           VIBPutChar(int c, int index);
void          VIBPutMsg(int index);
int           VIBGetChar(int index);
int           VIBMemoryMap(int index);
int           VIBMemoryUnMap(int index);
void          VIBInitVars(int index);
bool          VIBReadDataFromDevice(int *n, int index);
unsigned int  VIBGetRxErrorCount(int index);

#endif
