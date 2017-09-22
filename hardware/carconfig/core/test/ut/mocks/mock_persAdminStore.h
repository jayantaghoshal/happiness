/*
 * persAdminStore.h
 *
 *  Created on: Aug 19, 2016
 *      Author: bastian
 */

#ifndef CORE_TEST_UT_MOCKS_MOCK_PERSADMINSTORE_H_
#define CORE_TEST_UT_MOCKS_MOCK_PERSADMINSTORE_H_

#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>




typedef enum _PersASSelectionType_e
{
  PersASSelectionType_All          = 0,  /**< select all data/files: (node+user)->(application+shared) */
  PersASSelectionType_User         = 1,  /**< select user data/files: (user)->(application+shared) */
  PersASSelectionType_Application  = 2,  /**< select application data/files: (application)->(node+user) */
    /* insert new entries here ... */
  PersASSelectionType_LastEntry        /**< last entry */

} PersASSelectionType_e;


typedef enum _PersASDefaultSource_e
{
PersASDefaultSource_Factory         = 0, /**< select from factory definition */
PersASDefaultSource_Configurable    = 1, /**< select from user factory or configurable default if exist */
/* insert new entries here ... */
PersASDefaultSource_LastEntry /**< last entry */
} PersASDefaultSource_e;


#endif /* CORE_TEST_UT_MOCKS_MOCK_PERSADMINSTORE_H_ */
