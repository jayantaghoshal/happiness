/*
*  ===========================================================================
*  [2017]- Volvo Car Corporation
*  All Rights Reserved
*  LEGAL
*  NOTICE:  All information (including intellectual and technical concepts) contained herein is, and remains the
* property of or licensed
*  to Volvo Car Corporation. This information is proprietary to Volvo Car Corporation and may be covered by patents or
* patent applications.
*  This information is protected by trade secret or copyright law. Dissemination of this information or reproduction of
* this material is
*  strictly forbidden unless prior written permission is obtained from Volvo Car Corporation.
*/

#include "dim_linked_item.h"

namespace vcc {
namespace dim {

DimLinkedItem::DimLinkedItem(uint16_t id, uint16_t groupID, ItemType itemType, TextAlignment align,
                             TextTruncation trunc, ItemSelectable select)
    : itemType_(itemType), DimItem(id, groupID, align, trunc, select) {}

DimItem::ItemType DimLinkedItem::GetType() const { return itemType_; }

}  // namespace dim
}  // namespace vcc
