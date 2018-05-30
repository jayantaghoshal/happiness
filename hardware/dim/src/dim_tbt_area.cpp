/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "dim_tbt_area.h"
#include <assert.h>
#include "dim_item.h"
#include "dim_label.h"

#define LOG_TAG "dim_manager"
#include <log/log.h>

namespace vcc {
namespace dim {

DimTbtArea::DimTbtArea(std::unique_ptr<DimOperationData> inputData) {
    if (inputData != nullptr) {
        inputData_ = std::move(inputData);
    } else {
        ALOGE("Bad state: Constructed with invalid dimOperationData");
    }
}

void DimTbtArea::SetTypeActivationIdentifier(const DimTbtArea::TypeActivationIdentifier& identifier) {
    tAidentifier_ = identifier;
}

void DimTbtArea::GetData(std::vector<uint8_t>& data) {
    if (inputData_ != nullptr) {
        DimEncode(inputData_->GetItems(), data);
    } else {
        ALOGE("Invalid inputData_");
    }
}

void DimTbtArea::DimEncode(const std::vector<std::unique_ptr<DimItem>>& inData, std::vector<uint8_t>& outData) {
    outData.push_back(static_cast<uint8_t>(tAidentifier_));
    outData.push_back(static_cast<uint8_t>(0x00));
    assert(inData.size() < 256);
    outData.push_back(static_cast<uint8_t>(inData.size()));  // no of items
    std::vector<uint8_t> data;
    for (const auto& item : inData) {
        assert(item != nullptr);
        data = item->GetItemData(Connectivity::VccIpCmd::OperationId::DIMTbTArea);
        outData.insert(outData.end(), data.begin(), data.end());  // item data
    }
}

}  // namespace dim
}  // namespace vcc
