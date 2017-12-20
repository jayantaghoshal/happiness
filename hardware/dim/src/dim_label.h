/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef DIMLABEL_H
#define DIMLABEL_H

#include "dim_item.h"

namespace vcc {
namespace dim {

/**
 * @class DimLabel
 * @brief Implements label functionality.
 */
class DimLabel final : public DimItem {
  public:
    /**
     * @brief Constructor of DimLabel class.
     * Initializes item with provided parameters.
     * @param id[in] id of DimLabel
     * @param groupID[in] group id of DimLabel
     * @param align[in] sets text aligment position
     * @param trunc[in] sets text tructation position
     * @param select[in] indicates if item is selectable
     */
    DimLabel(uint16_t id, uint16_t groupID, TextAlignment align = TextAlignment::Alignment_Left,
             TextTruncation trunc = TextTruncation::Truncation_Right,
             ItemSelectable select = ItemSelectable::Selectable_True);

  protected:
    /**
     * @brief Returns item type
     * @return ItemType
     */
    ItemType GetType() const override;
};

}  // namespace dim
}  // namespace vcc

#endif
