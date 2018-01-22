/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "dim_item.h"

namespace vcc {
namespace dim {

/**
 * @class DimImage
 * @brief Implements image functionality.
 */
class DimImage final : public DimItem {
  public:
    /**
     * @brief Constructor of DimImage class.
     * Initializes item with provided parameters.
     * @param id[in] id of DimImage
     * @param groupID[in] group id of DimImage
     * @param align[in] sets text aligment position
     * @param trunc[in] sets text tructation position
     * @param select[in] indicates if item is selectable
     */
    DimImage(uint16_t id, uint16_t groupID, TextAlignment align = TextAlignment::Alignment_Center,
             TextTruncation trunc = TextTruncation::Truncation_Right,
             ItemSelectable select = ItemSelectable::Selectable_True);

  protected:
    /**
     * @brief Returns item type
     * @return ItemType
     */
    ItemType GetType() const override;
};
/** @} dimItem end group */
}  // namspace dim
}  // namespace vcc
