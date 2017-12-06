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

#ifndef DIMLINKEDITEM_H
#define DIMLINKEDITEM_H

#include "dim_item.h"

namespace vcc {
namespace dim {

/**
 * @class DimLinkedItem
 * @brief Implements linked item functionality.
 */
class DimLinkedItem final : public DimItem {
  public:
    /**
     * @brief Constructor of DimLinkedItem class.
     * Initializes item with provided parameters.
     * @param id[in] id of DimLinkedItem
     * @param groupID[in] group id of DimLinkedItem
     * @param itemType[in] sets dim item type
     * @param align[in] sets text alignment position
     * @param trunc[in] sets text trucation position
     * @param select[in] indicates if item is selectable
     */
    DimLinkedItem(uint16_t id, uint16_t groupID, ItemType itemType,
                  TextAlignment align = TextAlignment::Alignment_Center,
                  TextTruncation trunc = TextTruncation::Truncation_Right,
                  ItemSelectable select = ItemSelectable::Selectable_True);

  protected:
    /**
     * @brief Item type (linked image, linked animation, linked text)
     */
    ItemType itemType_;

    /**
     * @brief Returns item type
     * @return ItemType
     */
    ItemType GetType() const override;
};
/** @} dimItem end group */
}  // namespace dim
}  // namespace vcc

#endif  // !DIMLINKEDITEM_H
