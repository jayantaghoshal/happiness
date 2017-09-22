/*
 * mock_persAdminStore.cpp
 *
 *  Created on: Aug 19, 2016
 *      Author: bastian
 */


#include "persistence_admin_service.h"

extern long persAdminStoreReturn;
extern long persAdminStoreCntr;
extern int persAdminStoreLast;

long persAdminDataRestore(PersASSelectionType_e type, PersASDefaultSource_e defaultSource, const char* applicationID,
                unsigned int user_no, unsigned int seat_no)
{
    persAdminStoreCntr++;
    persAdminStoreLast = (int) user_no;
    return persAdminStoreReturn;
}


