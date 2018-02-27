#pragma once

#include <functional>
#include "notifiable_property.h"
#include "user_selection_factory.h"
#include "v0/org/volvocars/climate/UserSelection.hpp"

class UserSelectionFactory;

namespace v0 {
namespace org {
namespace volvocars {
namespace climate {

using GenericReply2 = std::function<void(CommonTypes::ReturnCode)>;

class UserSelectionStubImpl {
  public:
    UserSelectionStubImpl(UserSelectionFactory& logicFactory);
    virtual ~UserSelectionStubImpl() {}

    // Stub interface
  public:
    void initStubAdapter();

    // UserSelectionStub interface
  public:
    const UserSelection::OffOnSelection& getAutoDriverSeatHeatAttribute();
    const UserSelection::LevelSelection& getAutoDriverSeatHeatLevelAttribute();
    const UserSelection::OffOnSelection& getAutoPassengerSeatHeatAttribute();
    const UserSelection::LevelSelection& getAutoPassengerSeatHeatLevelAttribute();
    const UserSelection::OffOnSelection& getAutoSteeringWheelHeatAttribute();
    const UserSelection::LevelSelection& getAutoSteeringWheelHeatLevelAttribute();
    const UserSelection::OffOnSelection& getAirQualitySensorAttribute();
    const UserSelection::OffOnSelection& getManualRecircTimerAttribute();
    const UserSelection::OffOnSelection& getAutoFrontDefrosterAttribute();
    const UserSelection::PreconditioningHeatSourceStruct& getPreconditioningHeatSourceAttribute();
    const UserSelection::OffOnSelection& getAutoRearDefrosterAttribute();
    const UserSelection::OffOnSelection& getAdditionalHeaterAttribute();

    void requestAutoDriverSeatHeat(UserSelection::OffOnType _autoHeatSelection, GenericReply2 _reply);
    void requestAutoDriverSeatHeatLevel(UserSelection::LevelType _autoHeatLevel, GenericReply2 _reply);
    void requestAutoPassengerSeatHeat(UserSelection::OffOnType _autoHeatSelection, GenericReply2 _reply);
    void requestAutoPassengerSeatHeatLevel(UserSelection::LevelType _autoHeatLevel, GenericReply2 _reply);
    void requestAutoSteeringWheelHeat(UserSelection::OffOnType _autoHeatSelection, GenericReply2 _reply);
    void requestAutoSteeringWheelHeatLevel(UserSelection::LevelType _autoHeatLevel, GenericReply2 _reply);
    void requestAirQualitySensor(UserSelection::OffOnType _selection, GenericReply2 _reply);
    void requestManualRecircTimer(UserSelection::OffOnType _selection, GenericReply2 _reply);
    void requestAutoFrontDefroster(UserSelection::OffOnType _selection, GenericReply2 _reply);
    void requestPreconditioningHeatSource(UserSelection::PreconditioningHeatSourceEnum _selection,
                                          GenericReply2 _reply);
    void requestAutoRearDefroster(UserSelection::OffOnType _selection, GenericReply2 _reply);
    void requestAdditionalHeater(UserSelection::OffOnType _selection, GenericReply2 _reply);

  private:
    UserSelectionFactory& logicFactory_;

    std::vector<SubscriptionHandle> subscriptionHandles_;
};

}  // namespace climate
}  // namespace volvocars
}  // namespace org
}  // namespace v0
