/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "dim_item.h"
#include "dim_tools.h"

using namespace Connectivity;

namespace vcc {
namespace dim {

DimItem::DimItem(uint16_t id, uint16_t groupID, TextAlignment align, TextTruncation trunc, ItemSelectable select)
    : id_(id), groupID_(groupID), aligment_(align), truncation_(trunc), selectable_(select), rawData_() {}

void DimItem::SetRawData(std::vector<uint8_t> const& data) { rawData_ = data; }

void DimItem::SetRawData(std::string const& data) { rawData_ = dim_tools::DimTools::ConvertStringToHexData(data); }

std::vector<uint8_t> DimItem::GetItemData(VccIpCmd::OperationId fType) const {
    std::vector<uint8_t> itemData;
    if (fType == VccIpCmd::OperationId::DIMNotification) {
        itemData.push_back(static_cast<uint8_t>(id_));       // item identifier
        itemData.push_back(static_cast<uint8_t>(groupID_));  // item group
    } else if (fType == VccIpCmd::OperationId::DIMTbTArea) {
        itemData.push_back(static_cast<uint8_t>(id_));  // item identifier
    }

    itemData.insert(itemData.end(), {static_cast<uint8_t>(GetType()), static_cast<uint8_t>(aligment_),
                                     static_cast<uint8_t>(truncation_), static_cast<uint8_t>(selectable_)});
    uint32_t size = rawData_.size();
    itemData.insert(itemData.end(), {static_cast<uint8_t>(size >> 24), static_cast<uint8_t>(size >> 16),
                                     static_cast<uint8_t>(size >> 8), static_cast<uint8_t>(size)});  // length of item
    itemData.insert(itemData.end(), rawData_.begin(), rawData_.end());                               // item raw data

    return itemData;
}

void DimItem::SetRawData(const uint16_t& data) {
    rawData_.push_back(static_cast<uint8_t>(data >> 8));
    rawData_.push_back(static_cast<uint8_t>(data));
}

}  // namespace dim
}  // namespace vcc
