/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
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
