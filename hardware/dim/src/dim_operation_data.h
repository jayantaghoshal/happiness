/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
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
