/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "dim_image.h"

namespace vcc {
namespace dim {

DimImage::DimImage(uint16_t id, uint16_t groupID, TextAlignment align, TextTruncation trunc, ItemSelectable select)
    : DimItem(id, groupID, align, trunc, select) {}

DimItem::ItemType DimImage::GetType() const {
    return ItemType::Item_Image;
}

}  // namespace dim
}  // namespace vcc
