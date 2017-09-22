#ifndef CARCONFIG_UPDATER_H
#define CARCONFIG_UPDATER_H

#include <array>
#include <vector>
#include "carconfig_file.h"
#include "diagnostics_client.h"
#include "vipcom_client.h"
#include <map>

// The internal type that represent 504 received carconfig frames
typedef struct
{
    bool received = false;
    bool ok = false;
    uint8_t value = 0;
    bool subscribed = false;
} ccFlexrayValue;

typedef std::array<ccFlexrayValue, 504> ccBuffer;

extern bool debugMode;

void frameReceiver(ccBuffer &buff, uint32_t timeout);

bool fileExists(std::string filePath);

void writeEmptyFile(std::string filePath);

void checkReceivedValues(const std::map<int, std::vector<int>> ccParamList, ccBuffer &buff, bool &allParamsReceived,
                         bool &allParamsOK, std::map<uint32_t, uint8_t> &errorList);

void checkExistingParams(CarconfigFile &ccFile, const std::string ccFileName,
                         const std::map<int, std::vector<int>> ccParamList, bool &allParamsOK,
                         std::map<uint32_t, uint8_t> &errorList);

bool storeReceivedParameter(CarconfigFile &ccFile, std::string outputFile, ccBuffer &buffer);

std::vector<uint8_t> carconfigParamFaultsPack(std::map<uint32_t, uint8_t> params);

void updateVipParameters(CarconfigFile &ccFile, CarConfigVipCom &vipcomClient);

int32_t runUpdater(void);

#endif  // CARCONFIG_UPDATER_H
