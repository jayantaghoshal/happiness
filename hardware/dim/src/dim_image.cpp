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

#include "dim_image.h"

namespace vcc {
namespace dim {

DimImage::DimImage(uint16_t id, uint16_t groupID, TextAlignment align, TextTruncation trunc, ItemSelectable select)
    : DimItem(id, groupID, align, trunc, select) {}

DimItem::ItemType DimImage::GetType() const { return ItemType::Item_Image; }

}  // namespace dim
}  // namespace vcc
