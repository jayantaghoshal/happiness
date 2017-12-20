/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef DIMITEM_H
#define DIMITEM_H

#include <inttypes.h>
#include <string>
#include <vector>
#include "VccIpCmdApi.h"
#include "dim_tools.h"

namespace vcc {
namespace dim {

/**
 * @class DimItem
 * @brief Base class for all items.
 */
class DimItem {
  public:
    /**
     * @brief The TextAlignment enum
     */
    enum class TextAlignment { Alignment_Left = 0, Alignment_Center = 1, Alignment_Right = 2 };

    /**
     * @brief The TextTruncation enum
     */
    enum class TextTruncation { Truncation_Left = 0, Truncation_Right = 1 };

    /**
     * @brief The ItemSelectable enum
     */
    enum class ItemSelectable {
        Selectable_False = 0,
        Selectable_True = 1,
        Selectable_Highlighted = 2,
        Selectable_Activated = 3
    };

    /**
     * @brief The Itemtype enum
     */
    enum class ItemType {
        Item_Unknown = -1,
        Item_Image = 0,
        Item_Text = 1,
        Item_Animation = 2,
        Item_Linked_Image = 3,
        Item_Linked_Text = 4,
        Item_Linked_Animation = 5,
        Item_Video = 6,
        Item_Time_Elapsed = 7
    };

    /**
     * @brief Constructor of DimItem class.
     * Initializes item with provided parameters.
     * @param id[in] id of DimItem
     * @param groupID[in] group id of DimItem
     * @param align[in] sets text aligment position
     * @param trunc[in] sets text tructation position
     * @param select[in] indicates if item is selectable
     */
    DimItem(uint16_t id, uint16_t groupID, TextAlignment align, TextTruncation trunc, ItemSelectable select);

    /**
     * @brief Destructor of DimItem class.
     */
    virtual ~DimItem() {}

    /**
     * @brief Writes input vector into item raw data.
     * @param data[in]
     */
    void SetRawData(std::vector<uint8_t> const& data);

    /**
     * @brief Writes input string into item raw data.
     * @param data[in]
     */
    void SetRawData(std::string const& data);

    /**
     * @brief Writes input data (linked item ID) into item raw data.
     * @param data[in]
     */
    void SetRawData(uint16_t const& data);

    /**
     * @brief Returns item raw data.
     * @return vector<uint8_t>
     */
    std::vector<uint8_t> GetRawData() const { return rawData_; }

    /**
     * @brief Returns item data.
     * @return vector<uint8_t>
     */
    std::vector<uint8_t> GetItemData(Connectivity::VccIpCmd::OperationId fType) const;

  protected:
    /**
     * @brief Item ID.
     */
    uint16_t id_ = 0;

    /**
     * @brief Item group ID.
     */
    uint16_t groupID_ = 0;

    /**
     * @brief Item text alignment.
     */
    TextAlignment aligment_ = TextAlignment::Alignment_Center;

    /**
     * @brief Item text truncation.
     */
    TextTruncation truncation_ = TextTruncation::Truncation_Right;

    /**
     * @brief Item selectable configuration.
     */
    ItemSelectable selectable_ = ItemSelectable::Selectable_True;

    /**
     * @brief Item raw data.
     */
    std::vector<uint8_t> rawData_ = {};

    /**
     * @brief Pure virtual method for get item type.
     * @return ItemType
     */
    virtual ItemType GetType() const = 0;
};

}  // namespace dim
}  // namespace vcc
#endif
