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

#ifndef DIMOPERATIONDATA_H
#define DIMOPERATIONDATA_H

#include <memory>
#include <vector>
#include "dim_item.h"

namespace vcc {
namespace dim {

class DimOperationData {
  public:
    virtual ~DimOperationData(){};

    const std::vector<std::unique_ptr<DimItem>> &GetItems() const { return items_; }

    void AddItem(std::unique_ptr<DimItem> item);
    void AddLabel(uint16_t id, uint16_t group, const std::string &text);
    void AddImage(uint16_t id, uint16_t group, const std::vector<uint8_t> &imageData);
    void AddLinkedImage(uint16_t id, uint16_t group, uint16_t data);

  private:
    std::vector<std::unique_ptr<DimItem>> items_;
};

}  // namespace dim
}  // namespace vcc

#endif
