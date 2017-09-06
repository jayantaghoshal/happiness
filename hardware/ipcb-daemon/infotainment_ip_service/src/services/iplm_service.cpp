/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#define LOG_TAG "iplm_service"

#include "iplm_service.h"

#include <ipcommandbus/MessageDispatcher.h>
#include <cutils/log.h>

#include "util/local_config.h"
#include "services/diagnostics_client.h"

using namespace tarmac::eventloop;

namespace
{
// Ref:: https://delphisweden.atlassian.net/wiki/display/VI/NSM+-+Session+-+IPLM+Resource+Groups
const char* kIPLMSessionName = "IPLM_ResourceGroups";
const char* kIPLMSessionOwner = "IPLM";

// This variable is used as a gatekeeper before updates are made towards NSM. It is important that after IPLM
// initialization, IPLM shall wait for RemoteMonitoringTmo period or activity message from remote nodes, before
// initiating flexray wakeup or NSM update
bool first_contact = false;

// NOTE: Outside requirement document. Verbal agreement/discussion with Karl Ronqvist (VCC).
// Flexray wakeup shall be attempted only once per InfotainmentMode change cycle i.e. IM_OFF --> IM_ON --> IM_OFF
bool flexray_wakeup_attempted = false;

enum NsmStateBitMask : std::size_t
{
    MP_IPLM_BIT_EXT_GR_1 = 2,
    MP_IPLM_BIT_EXT_GR_3 = 3,
    MP_IPLM_BIT_EXT_PRIORITY = 4,
    MP_IPLM_BIT_VALUE = 5,
    MP_IPLM_BIT_INT_PRIORITY = 6,
};
}

namespace utils = InfotainmentIpService::Utils;

namespace Connectivity
{
IpLmServiceStubImpl::IpLmServiceStubImpl(IDispatcher& timeProvider)
    : timeProvider{timeProvider}
{
    service_name_ = "IPLM";
}

//
// Public methods
//
bool IpLmServiceStubImpl::Initialize(Connectivity::MessageDispatcher* msgDispatcher)
{
    setDispatcher(msgDispatcher);

    // For initial registration with NSM; we use state with bit5 reset to indicate that State used in registration
    // in invalid and shall be ignored by VIP PowerModing.  bit0 and bit1 shall always be set.
    // Ref:: https://delphisweden.atlassian.net/wiki/display/VI/NSM+-+Session+-+IPLM+Resource+Groups
    //    bit0: Has to be always 1 - NSM legacy code dependency
    //    bit1: Has to be always 1 - NSM legacy code dependency
    //    bit2: External RG1 (0:inactive 1:active)
    //    bit3: External RG3 (0:inactive 1:active)
    //    bit4: External Priority (0:low 1:high)
    //    bit5: IPLM Value (0:invalid 1:valid)
    //    bit6: Internal Priority (0:low 1:high)

    /* TODO handle state/shutdown etc
    const int InitialNsmState = 0b00000011;
    int registration_status =
        nsmState_.RegisterSession(kIPLMSessionName, kIPLMSessionOwner, NsmSeat_Driver, InitialNsmState);

    // How to handle error -1 which means fails to communicate with NSM. Ponder
    if (registration_status != NsmErrorStatus_Ok)
    {
        int session_state = nsmState_.GetSessionState(kIPLMSessionName, NsmSeat_Driver);
        // How to handle error -1 which means fails to communicate with NSM. Ponder
        if (NsmSessionState_Unregistered == session_state)
        {
            ALOGE("Failed to Register NSM sessions for IPLM.");
            return false;
        }
        // session_state != Unregistered; indicates that a valid session for IPLM exists (this can happen if for e.g.
        // service crash/restart or during development developer manually stop/start service)
    }
    */

    // IPLM is interested in receiving IP_activity messages which are broadcasted by remote ECUs in Car
    // Ref REQPROD 347119
    registerNotificationCyclicCallback(VccIpCmd::ServiceId::LM,
                                       VccIpCmd::OperationId::IpActivity,
                                       std::bind(&IpLmServiceStubImpl::CbLmBroadcast, this, std::placeholders::_1));

    restartTemActivityTimer();
    restartVcmActivityTimer();

    // A recurrent timer used to send IHU's IP activity messages
    // Timeout as specified in REQPROD 347839
    activityTimer_ = timeProvider.EnqueueWithDelay(std::chrono::milliseconds(1000), [this]() { ActivityTimeout(); });

    return true;
}

void IpLmServiceStubImpl::Uninitialize()
{
    timeProvider.Cancel(activityTimer_);
    timeProvider.Cancel(activityVCM_);
    timeProvider.Cancel(activityTEM_);
}

void IpLmServiceStubImpl::Request(std::string _serviceName,
                                  XResourceGroup _rg,
                                  XResourcePrio _prio)
{
    ALOGI("Request called for service (%s) and RG (%s) and prio (%s)",
          _serviceName.c_str(),
          ToString(_rg),
          ToString(_prio));

    // IHU is part of RG1 or RG3. Filter out requests for other RGs
    if (!IsRequestedRGValid(_rg))
    {
        ALOGW("unexpected resource group %d. Ignored. TODO reply", _rg);
        //_reply();
        return;
    }

    const ResourceGroup rg = (XResourceGroup::kResourceGroup1 == _rg) ? ResourceGroup::RG_1 : ResourceGroup::RG_3;
    const Prio prio = (XResourcePrio::kPrioNormal == _prio) ? PRIO_NORM : PRIO_HIGH;

    // Look for registered service and update requested parameters corresponding to service
    ServicePrioMap::iterator it;
    if (!IsServiceRegistered(iplm_data_, _serviceName, it))
    {
        ALOGW("RG request from unregistered service ignored. TODO reply");
        //_reply();
        return;
    }

    // update requested rg in per service record
    it->second.rg_ = static_cast<ResourceGroup>(rg | it->second.rg_);
    // update requested prio in per service record
    (rg & ResourceGroup::RG_1) ? (it->second.prio_rg1_ = prio) : (it->second.prio_rg3_ = prio);

    // Set requested RG in action_ so that it is included in activity messages sent by IHU
    iplm_data_.action_[Message::Ecu::IHU] |= rg;
    if (prio == PRIO_HIGH)
    {
        iplm_data_.prio_[Message::Ecu::IHU] = PRIO_HIGH;
    }
    else if (!IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_1, PRIO_HIGH) &&
             !IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_3, PRIO_HIGH))
    {
        iplm_data_.prio_[Message::Ecu::IHU] = PRIO_NORM;
    }

    SetNsmSessionState();
    SendFlexrayWakeup(rg, prio);

    //_reply(); TODO
}

void IpLmServiceStubImpl::Release(std::string _serviceName, XResourceGroup _rg)
{
    ALOGI("Release called for service (%s) and RG (%s)", _serviceName.c_str(), ToString(_rg));

    // IHU is part of RG1 or RG3. Filter out requests for other RGs
    if (!IsRequestedRGValid(_rg))
    {
        ALOGW("Unexpected resource group %d. Ignored TODO reply", _rg);
        //_reply();
        return;
    }

    const ResourceGroup rg = (XResourceGroup::kResourceGroup1 == _rg) ? ResourceGroup::RG_1 : ResourceGroup::RG_3;

    ServicePrioMap::iterator it;
    if (!IsServiceRegistered(iplm_data_, _serviceName, it))
    {
        ALOGW("RG request from unregistered service Ignored TODO reply()");
        //_reply();  // Unregistered service requesting release for RG. Ignore...
        return;
    }

    // Reset corresponding RG bit for the LSC
    it->second.rg_ = static_cast<ResourceGroup>(it->second.rg_ & (~rg));
    // Reset prio for the LSC
    (rg & ResourceGroup::RG_1) ? (it->second.prio_rg1_ = PRIO_NORM) : (it->second.prio_rg3_ = PRIO_NORM);

    // If no LSC is requesting high prio for any RG locally; reset HIGH PRIO for IHU
    if (!IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_1, PRIO_HIGH) &&
        !IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_3, PRIO_HIGH))
    {
        iplm_data_.prio_[Message::Ecu::IHU] = PRIO_NORM;
        SetNsmSessionState();
    }

    // Check if any LSC is still interested in this particular resource_group
    if (!IsRgRequestedLocally(iplm_data_, rg))
    {
        // as no other LSC is requesting this RG; reset corresponding bit
        iplm_data_.action_[Message::Ecu::IHU] &= (~rg);
    }

    //_reply(); TODO
}

void IpLmServiceStubImpl::RegisterService(std::string _serviceName)
{
    ALOGI("RegisterService: called for service (%s)", _serviceName.c_str());

    // register a new service
    if (!IsServiceRegistered(iplm_data_, _serviceName))
    {
        // Insert new service to registered LSCs list
        iplm_data_.registered_LSCs_.insert(ServicePrioMap::value_type(_serviceName, RGRequestInfo()));

        // If all LSCs are registered; mark ACTION_AVAILABLE to start IP_activity broadcast. Ref: REQPROD 347878
        if (utils::getNofLocalSoftwareComponents() <= static_cast<int>(iplm_data_.registered_LSCs_.size()))
        {
            iplm_data_.action_[Message::Ecu::IHU] |= ACTION_AVAILABLE;

            // As Local IPLinkManager is now available; we should start monitoring remote nodes for activity
            restartVcmActivityTimer();
            restartTemActivityTimer();
        }
    }
    else
    {
        ALOGW("registration request from already registered service (%s) ", _serviceName.c_str());
    }

    //_reply(); TODO
}

void IpLmServiceStubImpl::restartVcmActivityTimer()
{
    // RequestMonitoringTimeout is configurable through Local config as required by REQPROD 347880
    activityVCM_ = timeProvider.EnqueueWithDelay(std::chrono::milliseconds(utils::getRequestMonitoringTmo() + 100),
                                                      [this]() { RequestMonitoringTimeout(EcuId::ECU_Vcm); });
}
void IpLmServiceStubImpl::restartTemActivityTimer()
{
    // RequestMonitoringTimeout is configurable through Local config as required by REQPROD 347880
    activityTEM_ = timeProvider.EnqueueWithDelay(std::chrono::milliseconds(utils::getRequestMonitoringTmo() + 100),
                                                      [this]() { RequestMonitoringTimeout(EcuId::ECU_Tem); });
}

void IpLmServiceStubImpl::UnRegisterService(std::string _serviceName)
{
    ALOGI("UnRegisterService: called for service (%s)", _serviceName.c_str());

    ServicePrioMap::iterator it;
    if (!IsServiceRegistered(iplm_data_, _serviceName, it))
    {
        ALOGW("UnRegistration request from a not registered service (%s) TODO reply", _serviceName.c_str());
        //_reply();
        return;
    }

    iplm_data_.registered_LSCs_.erase(it);

    // If all LSCs are not registered; reset ACTION_AVAILABLE to STOP IP_activity broadcast. Ref: REQPROD 347878
    if (utils::getNofLocalSoftwareComponents() > static_cast<int>(iplm_data_.registered_LSCs_.size()))
    {
        iplm_data_.action_[Message::Ecu::IHU] |= (~ACTION_AVAILABLE);
    }

    if (!IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_1, PRIO_HIGH) &&
        !IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_3, PRIO_HIGH))
    {
        // No LSC needs HIGH prio on RG1 or RG3
        iplm_data_.prio_[Message::Ecu::IHU] = PRIO_NORM;
        SetNsmSessionState();
    }

    if (!IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_1))
    {
        // reset corresponding RG bit as no other LSCs wants this RG
        iplm_data_.action_[Message::Ecu::IHU] &= (~ResourceGroup::RG_1);
    }

    if (!IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_3))
    {
        // reset corresponding RG bit as no other LSCs wants this RG
        iplm_data_.action_[Message::Ecu::IHU] &= (~ResourceGroup::RG_3);
    }

    //_reply(); TODO
}

bool IpLmServiceStubImpl::GetEcuAvailibility(const Message::Ecu& ecu, uint8_t& resourceGroup, uint8_t& prio)
{
    if (ecu == Message::Ecu::UNKNOWN || ecu > Message::Ecu::IHU)
    {
        ALOGW("Specified unknown or out of bounds ECU: %s", Message::EcuStr(ecu));
        resourceGroup = 0;
        prio = 0;
        return false;
    }

    resourceGroup = iplm_data_.action_[static_cast<int>(ecu)];
    prio = iplm_data_.prio_[static_cast<int>(ecu)];

    ALOGI("ECU= %s, ACTION= %s, PRIO= %s", Message::EcuStr(ecu), ToString(resourceGroup).c_str(), ToString(prio).c_str());

    return true;
}

//
// Private methods
//
void IpLmServiceStubImpl::CbLmBroadcast(Message& msg)
{
    ALOGV("CbLmBroadcast %04X.%04X.%02d 0x%08X(size: %d)",
                                 (int)msg.pdu.header.service_id,
                                 (int)msg.pdu.header.operation_id,
                                 (int)msg.pdu.header.operation_type,
                                 msg.pdu.header.sender_handle_id,
                                 (int)msg.pdu.payload.size());

    if (VccIpCmd::OperationType::ERROR == msg.pdu.header.operation_type)
        return;

    assert(msg.ecu <= Message::Ecu::ENUM_NR_OF_ECUs);

    first_contact = true;
    ALOGV("Got IP_Activity(%s,%s) from %s",
          ToString(static_cast<Action>(msg.pdu.payload.data()[0])).c_str(),
          ToString(static_cast<Prio>(msg.pdu.payload.data()[1])),
          Message::EcuStr(msg.ecu));

    if (iplm_data_.action_[Message::Ecu::IHU] & ACTION_AVAILABLE)
    {
        if (msg.ecu == Message::Ecu::VCM)
            timeProvider.Cancel(activityVCM_);
        else
            timeProvider.Cancel(activityTEM_);
    }

    iplm_data_.action_[msg.ecu] = static_cast<Action>(msg.pdu.payload.data()[0]);
    iplm_data_.prio_[msg.ecu] = static_cast<Prio>(msg.pdu.payload.data()[1]);

    iplm_data_.rg1_availabilityStatus_.set(
        static_cast<int>((msg.ecu == Message::Ecu::VCM) ? EcuId::ECU_Vcm : EcuId::ECU_Tem));
    iplm_data_.rg3_availabilityStatus_.set(
        static_cast<int>((msg.ecu == Message::Ecu::VCM) ? EcuId::ECU_Vcm : EcuId::ECU_Tem));

    SetNsmSessionState();

    if (iplm_data_.action_[Message::Ecu::IHU] & ACTION_AVAILABLE)
    {
        if (msg.ecu == Message::Ecu::VCM)
            restartVcmActivityTimer();
        else
            restartTemActivityTimer();
    }
}

void IpLmServiceStubImpl::CreateAndSendIpActivityMessage()
{
    ALOGD("LM: Timeout, prepare new broadcast message");

    Action action = iplm_data_.action_[Message::Ecu::IHU];
    Prio prio = iplm_data_.prio_[Message::Ecu::IHU];

    // Activity messages shall be sent only when ACTION_AVAILABLE bit is set. REQPROD 347878
    if ((action & ACTION_AVAILABLE) != ACTION_AVAILABLE)
        return;

    Pdu pdu;
    // Prepare header. REQPROD:347119
    pdu.createHeader(VccIpCmd::ServiceId::LM,
                     VccIpCmd::OperationId::IpActivity,
                     VccIpCmd::OperationType::NOTIFICATION_CYCLIC,
                     VccIpCmd::DataType::NOT_ENCODED,
                     getNewSequenceId());

    pdu.setPayload({action, (uint8_t)prio, 0, 0});

    // Create message and set pdu.
    Message msg(std::move(pdu));

    ALOGD("Send IP_Activity(%s,%s)", ToString(action).c_str(), ToString(prio));

    broadcastMessage(std::move(msg));
}

void IpLmServiceStubImpl::RequestMonitoringTimeout(EcuId ecu)
{
    // No IP Activity message received from remote node in last request_monitoring_time. Node died may be?

    assert(ecu == EcuId::ECU_Vcm || ecu == EcuId::ECU_Tem);

    first_contact = true;

    ALOGD("Timeout on IP_Activity for %d", ecu);

    iplm_data_.rg1_availabilityStatus_.reset(static_cast<int>(ecu));
    iplm_data_.rg3_availabilityStatus_.reset(static_cast<int>(ecu));

    // Node not available. Reset AVAILABLE bit in action_ corresponding to ECU
    (ecu == EcuId::ECU_Vcm) ? (iplm_data_.action_[static_cast<int>(Message::Ecu::VCM)] = 0)
                            : (iplm_data_.action_[static_cast<int>(Message::Ecu::TEM)] = 0);

    SendFlexrayWakeup(ResourceGroup::RG_1, iplm_data_.prio_[Message::Ecu::IHU]);
}

void IpLmServiceStubImpl::ActivityTimeout()
{
    bool broadcast_allowed = false;
    bool tem_available = false;
    bool vcm_available = false;
    auto rg1_status = XResourceGroupStatus::kResourceGroupStatus_Available;

    broadcast_allowed =
        (utils::getNofLocalSoftwareComponents() <= static_cast<int>(iplm_data_.registered_LSCs_.size()));

    rg1_status = iplm_data_.rg1_availabilityStatus_.all()
                     ? XResourceGroupStatus::kResourceGroupStatus_Available
                     : iplm_data_.rg1_availabilityStatus_.any()
                           ? XResourceGroupStatus::kResourceGroupStatus_PartlyAvailable
                           : XResourceGroupStatus::kResourceGroupStatus_Unavailable;
    tem_available = iplm_data_.rg1_availabilityStatus_.test(static_cast<int>(EcuId::ECU_Tem));
    vcm_available = iplm_data_.rg1_availabilityStatus_.test(static_cast<int>(EcuId::ECU_Vcm));

    if (broadcast_allowed)
    {
        // time to send new activity message
        CreateAndSendIpActivityMessage();
    }

    // send events to LSCs. Update is sent periodically every second
    /* TODO
    iplm_service_stub_->fireRgStatusEvent(generated::IpLmService::ResourceGroup::kResourceGroup1, rg1_status);
    iplm_service_stub_->fireRgStatusEvent(generated::IpLmService::ResourceGroup::kResourceGroup3, rg1_status);
    iplm_service_stub_->fireNodeStatusEvent(generated::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kTem, tem_available);
    iplm_service_stub_->fireNodeStatusEvent(generated::IpCommandBusTypes::Infotainment_IP_bus_Ecu::kVcm, vcm_available);
    */
    ALOGI("Tem available: %d",tem_available);
    ALOGI("Vcm available: %d",vcm_available);
    for (auto const& notifyAvailabilityToRegisteredService : node_availability_notifications_)
    {
        notifyAvailabilityToRegisteredService(EcuId::ECU_Tem, tem_available);
        notifyAvailabilityToRegisteredService(EcuId::ECU_Vcm, vcm_available);
    }
}

void IpLmServiceStubImpl::FlexrayWakeupTimeout()
{
    /* TODO
    autosar::NetActvtRec1 wakeup;
    wakeup.ResourceGroup = 0;
    wakeup.Prio = autosar::PrioHighNormal::PrioNormal;

    iplm_data_.senderWakeup_.send(wakeup);
    */
    ALOGI("FlexrayWakeupTimeout TODO");
}

bool IpLmServiceStubImpl::SendFlexrayWakeup(ResourceGroup _rg, Prio _prio)
{
    // If any LSC locally has requested RG to be kept active and RG is unavailable; time to request wakeup
    if (((iplm_data_.action_[Message::Ecu::IHU] & ACTION_AVAILABLE) != ACTION_AVAILABLE) ||
        iplm_data_.rg1_availabilityStatus_.all() || !first_contact || flexray_wakeup_attempted)
    {
        ALOGW(
            "Flexray wakeup not sent "
            "(registeredServices [%d], rg1_availabilityStatus_ [%d]), first_contact[%d], flexray_wakeup_attempted[%d]",
            (int)iplm_data_.registered_LSCs_.size(),
            iplm_data_.rg1_availabilityStatus_.all(),
            first_contact,
            flexray_wakeup_attempted);

        return false;
    }

    // Signal format as per REQPROD 347897
    // TODO
    //autosar::NetActvtRec1 wakeup;
    //wakeup.ResourceGroup = _rg;
    //wakeup.Prio = _prio == PRIO_HIGH ? autosar::PrioHighNormal::PrioHigh : autosar::PrioHighNormal::PrioNormal;

    auto tmoHandler = std::bind(&IpLmServiceStubImpl::FlexrayWakeupTimeout, this);
    //iplm_data_.senderWakeup_.send(wakeup);

    // Timeout as specified in REQPROD 347896
    //ApplicationDataElement::DESender<ActivateVfc_info> activatevfc;
    //activatevfc.send({Vfc::IPWakeup, 3});
    flexray_wakeup_attempted = true;

    iplm_data_.senderWakeupTimer_ = timeProvider.EnqueueWithDelay(std::chrono::milliseconds(3000), tmoHandler);
    ALOGI("SendFlexrayWakeup rg=%d , prio=%d  TODO",_rg,_prio);
    return true;
}

// Reference: https://delphisweden.atlassian.net/wiki/display/VI/NSM+-+Session+-+IPLM+Resource+Groups
bool IpLmServiceStubImpl::SetNsmSessionState()
{
    if ((iplm_data_.action_[Message::Ecu::IHU] & ACTION_AVAILABLE) != ACTION_AVAILABLE || !first_contact)
        return false;

    // bit0 reserved and set to true for legacy reasons
    // bit1 reserved and set to true for legacy reasons
    std::bitset<8> state = 0b00000011;

    // bit2 external RG1
    state.set(NsmStateBitMask::MP_IPLM_BIT_EXT_GR_1, IsRgRequestedByExternalNode(iplm_data_, ResourceGroup::RG_1));
    // bit3 external RG3
    state.set(NsmStateBitMask::MP_IPLM_BIT_EXT_GR_3, IsRgRequestedByExternalNode(iplm_data_, ResourceGroup::RG_3));
    // bit4 external prio
    state.set(NsmStateBitMask::MP_IPLM_BIT_EXT_PRIORITY, GetExternalPrio(iplm_data_));
    // bit5 iplm value
    state.set(NsmStateBitMask::MP_IPLM_BIT_VALUE, true);
    // bit6 internal prio
    state.set(NsmStateBitMask::MP_IPLM_BIT_INT_PRIORITY,
              IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_1, PRIO_HIGH));

    ALOGD("NSM IPLM Session state: %s TODO", state.to_string().c_str());

    //auto retVal = nsmState_.SetSessionState(kIPLMSessionName, kIPLMSessionOwner, NsmSeat_Driver, state.to_ulong());

    // How to handle error -1 which means fails to communicate with NSM. Ponder
    /*
    if (retVal < 0 || NsmErrorStatus_Ok != retVal)
    {
        ALOGE("Failed to set NSM state with reason %d", retVal);
        return false;
    }
    */
    return true;
}

//
// Helper functions for friendly debug output
//

const char* IpLmServiceStubImpl::ToString(const ResourceGroup r)
{
    switch (r)
    {
        case ResourceGroup::RG_Reserved:
            return "RG_Reserved";
        case ResourceGroup::RG_1:
            return "RG_1";
        case ResourceGroup::RG_3:
            return "RG_3";
        default:
            ALOGW("ToString() called with unsupported ResourceGroup");
            return "";
    }
}

const char* IpLmServiceStubImpl::ToString(const XResourceGroup r)
{
    switch (r)
    {
        case XResourceGroup::kResourceGroup1:
            return "RG_1";
        case XResourceGroup::kResourceGroup3:
            return "RG_3";
        default:
            ALOGW("ToString() called with unsupported ResourceGroup");
            return "";
    }
}

const std::string IpLmServiceStubImpl::ToString(const Action a)
{
    std::string s;

    if (a & ACTION_AVAILABLE)
    {
        s += std::string("AVAILABLE ");
    }
    if (a & ResourceGroup::RG_1)
    {
        s += std::string("RG_1 ");
    }
    if (a & ResourceGroup::RG_3)
    {
        s += std::string("RG_3 ");
    }
    return s;
}

const char* IpLmServiceStubImpl::ToString(const Prio a)
{
    switch (a)
    {
        case PRIO_NORM:
            return "NORM";
        case PRIO_HIGH:
            return "HIGH";
        default:
            ALOGW("ToString() called with unsupported Prio");
            return "";
    }
}

const char* IpLmServiceStubImpl::ToString(const XResourcePrio prio)
{
    switch (prio)
    {
        case XResourcePrio::kPrioNormal:
            return "NORM";
        case XResourcePrio::kPrioHigh:
            return "HIGH";
        default:
            ALOGW("ToString() called with unsupported Prio");
            return "";
    }
}

}  // namespace Connectivity
