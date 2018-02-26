#pragma once

#include "first_row_factory.h"
#include "notifiable_property.h"

class FirstRowFactory;

namespace common
{
namespace daemon
{
class Factory;
}
}

namespace v0
{
namespace org
{
namespace volvocars
{
namespace climate
{


using GenericReply=std::function<void(CommonTypes::ReturnCode)>;

class FirstRowStubImpl 
{
public:
    FirstRowStubImpl(FirstRowFactory& logicFactory, common::daemon::Factory& commonFactory);
    virtual ~FirstRowStubImpl() = default;
    void initStubAdapter();

    // ClimateStub interface
public:    
    const FirstRow::CleanzoneState& getCleanzoneAttribute() ;
    const FirstRow::AirConditionerState&
    getAirConditionerAttribute() ;
    const FirstRow::AirDistributionAngle&
    getAirDistributionAttribute() ;
    const FirstRow::FanLevelFrontValue&
    getFanLevelFrontAttribute() ;
    const FirstRow::MaxDefrosterState&
    getMaxDefrosterAttribute() ;
    const FirstRow::AutoClimateState&
    getAutoClimateAttribute() ;
    const FirstRow::ManualRecircState&
    getManualRecircAttribute() ;
    const FirstRow::ElectricDefrosterRearState&
    getElectricDefrosterRearAttribute() ;
    const FirstRow::ElectricDefrosterWindscreenState&
    getElectricDefrosterWindscreenAttribute() ;
    const FirstRow::ElectricDefrosterPopupState&
    getElectricDefrosterWindscreenPopupAttribute() ;
    const FirstRow::ElectricDefrosterPopupState&
    getElectricDefrosterRearPopupAttribute() ;
    const FirstRow::TemperatureAttribute&
    getDriverTemperatureAttribute() ;
    const FirstRow::TemperatureAttribute&
    getPassengerTemperatureAttribute() ;
    const FirstRow::TemperatureSyncState&
    getTemperatureSyncAttribute() ;
    const FirstRow::HeatAttribute&
    getSeatHeatDriverAttribute() ;
    const FirstRow::HeatAttribute&
    getSeatHeatPassengerAttribute() ;
    const FirstRow::HeatAttribute&
    getSteeringWheelHeatAttribute() ;
    const FirstRow::VentAttribute&
    getSeatVentDriverAttribute() ;
    const FirstRow::VentAttribute&
    getSeatVentPassengerAttribute() ;
    const FirstRow::IonizerState& getIonizerAttribute() ;

protected:
    
private:    
    FirstRowFactory&         logicFactory_;
    common::daemon::Factory& commonFactory_;

    // ClimateStub interface
public:
    void requestAirConditioner(
                               FirstRow::AirConditionerRequest            _value,
                               GenericReply               _reply) ;
    void requestClimateReset(
                             GenericReply                 _reply) ;
    void requestFanLevelFront(
                              FirstRow::FanLevelFrontRequest             _value,
                              GenericReply                _reply) ;
    void requestAutoClimate(
                            GenericReply                  _reply) ;
    void requestManualRecirc(
                             FirstRow::ManualRecircRequest              _value,
                             GenericReply                 _reply) ;
    void requestAirDistribution(
                                FirstRow::AirDistributionAngleRequest      _value,
                                GenericReply              _reply) ;
    void requestElectricDefrosterWindscreen(
                                            FirstRow::ElectricDefrosterWindscreenRequest _value,
                                            GenericReply    _reply) ;
    void requestElectricDefrosterWindscreenPopup(
                                                 FirstRow::ElectricDefrosterPopupRequest        _value,
                                                 GenericReply _reply) ;
    void requestElectricDefrosterRear(
                                      FirstRow::ElectricDefrosterRearRequest     _value,
                                      GenericReply        _reply) ;
    void requestElectricDefrosterRearPopup(
                                           FirstRow::ElectricDefrosterPopupRequest    _value,
                                           GenericReply   _reply) ;
    void requestMaxDefroster(
                             FirstRow::MaxDefrosterRequest              _value,
                             GenericReply                 _reply) ;
    void requestDriverTemperature(
                                  double                                     _value,
                                  GenericReply            _reply) ;
    void requestPassengerTemperature(
                                     double                                     _value,
                                     GenericReply         _reply) ;
    void requestTemperatureSync(
                                FirstRow::TemperatureSyncRequest           _value,
                                GenericReply              _reply) ;
    void requestSeatHeatDriver(
                               FirstRow::HeatLevel                        _level,
                               GenericReply               _reply) ;
    void requestSeatHeatPassenger(
                                  FirstRow::HeatLevel                        _level,
                                  GenericReply            _reply) ;
    void requestSteeringWheelHeat(
                                  FirstRow::HeatLevel                        _level,
                                  GenericReply            _reply) ;
    void requestSeatVentDriver(
                               FirstRow::VentLevel                        _level,
                               GenericReply               _reply) ;
    void requestSeatVentPassenger(
                                  FirstRow::VentLevel                        _level,
                                  GenericReply            _reply) ;
    void requestIonizer(
                        FirstRow::IonizerRequest                   _value,
                        GenericReply                      _reply) ;

private:
    std::vector<SubscriptionHandle> subscriptionHandles_;
};

} // namespace climate
} // namespace volvocars
} // namespace org
} // namespace v0
