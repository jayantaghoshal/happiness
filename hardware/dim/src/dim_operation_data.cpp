/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "dim_operation_data.h"
#include <cutils/log.h>
#include "dim_image.h"
#include "dim_item.h"
#include "dim_label.h"
#include "dim_linked_item.h"

namespace vcc {
namespace dim {

void DimOperationData::AddItem(std::unique_ptr<DimItem> item) {
    if (item != nullptr) {
        items_.push_back(std::move(item));
    } else {
        ALOGE("Invalid DIMItem. What should I do?");
    }
}

void DimOperationData::AddLabel(uint16_t id, uint16_t group, const std::string& text) {
    auto label = std::make_unique<DimLabel>(id, group);
    label->SetRawData(text);
    AddItem(std::move(label));
}

void DimOperationData::AddImage(uint16_t id, uint16_t group, const std::vector<uint8_t>& imageData) {
    auto image = std::make_unique<DimImage>(id, group);
    image->SetRawData(imageData);
    AddItem(std::move(image));
}

void DimOperationData::AddLinkedImage(uint16_t id, uint16_t group, uint16_t data) {
    auto linked = std::make_unique<DimLinkedItem>(id, group, DimItem::ItemType::Item_Linked_Image);
    linked->SetRawData(data);
    AddItem(std::move(linked));
}

}  // namespace dim
}  // namespace vcc
