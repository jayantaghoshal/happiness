/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "dim_linked_item.h"

namespace vcc {
namespace dim {

DimLinkedItem::DimLinkedItem(uint16_t id,
                             uint16_t groupID,
                             ItemType itemType,
                             TextAlignment align,
                             TextTruncation trunc,
                             ItemSelectable select)
    : itemType_(itemType), DimItem(id, groupID, align, trunc, select) {}

DimItem::ItemType DimLinkedItem::GetType() const { return itemType_; }

}  // namespace dim
}  // namespace vcc
