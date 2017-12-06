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
