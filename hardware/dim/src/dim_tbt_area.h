/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef DIMTBTAREA_H
#define DIMTBTAREA_H

#include <memory>
#include <vector>
#include "dim_item.h"
#include "dim_operation_data.h"

namespace vcc {
namespace dim {

/**
 * @brief The DimTbtArea (DimTurnByTurnArea) class
 * Encapsulates of Turn by turn operations
 */
class DimTbtArea final : public DimOperationData {
  public:
    /**
     * @brief TypeActivationIdentifier identifier enum
     */
    enum class TypeActivationIdentifier : std::uint8_t { INACTIVE = 0x00, ACTIVE = 0x01, CLOSETOMANEUVER = 0x02 };
    /**
     * @brief DimTbtArea Constructor
     * @param inputData
     */
    DimTbtArea(std::unique_ptr<DimOperationData> inputData);

    void SetTypeActivationIdentifier(const TypeActivationIdentifier& identifier);
    void GetData(std::vector<uint8_t>& data);
    void DimEncode(const std::vector<std::unique_ptr<DimItem>>& inData, std::vector<uint8_t>& outData);

  private:
    std::unique_ptr<DimOperationData> inputData_;
    TypeActivationIdentifier tAidentifier_ = TypeActivationIdentifier::INACTIVE;
};

}  // namespace dim
}  // namespace vcc

#endif
