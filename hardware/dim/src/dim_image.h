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

#ifndef DIMIMAGE_H
#define DIMIMAGE_H

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
#endif  //! DIMIMAGE_H
