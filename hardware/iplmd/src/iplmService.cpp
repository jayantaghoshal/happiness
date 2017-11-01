#include "iplmService.h"
#include "local_config.h"

#define LOG_TAG "iplmd.service"

using ::vendor::volvocars::hardware::vehiclecom::V1_0::OperationType;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::RetryInfo;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::CommandResult;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::SubscribeResult;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::Msg;
using ::vendor::volvocars::hardware::common::V1_0::Ecu;

namespace LocalCfg = Iplmd::LocalConfig;

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

IplmService::IplmService() :
    timeProvider_{IDispatcher::GetDefaultDispatcher()}
{
    Iplmd::LocalConfig::loadLocalConfig();

    StartSubscribe();

    IIplm::registerAsService(); // register as handler of the IIplm hidl interface
}

void IplmService::StartSubscribe()
{
    ipcbServer_ = IVehicleCom::getService("iplm");

    if (ipcbServer_ != NULL)
    {
        ALOGD("Ipcb HAL with name 'iplm' found! Register subscriber!");

        SubscribeResult result;
        // Install callback
        //TODO: Handle subscriber ID returned from subscribe?
        ipcbServer_.get()->subscribe(0xFFFF, 0xFF01,
                                     OperationType::NOTIFICATION_CYCLIC, this,
                                     [&result](SubscribeResult sr) { result = sr; });
        if (!result.commandResult.success)
        {
            ALOGE("Subscribe failed with error: %s", result.commandResult.errMsg.c_str());
        }

        Initialize();
    }
    else
    {
        ALOGD("Ipcb HAL with name 'iplm' not found in binder list, retrying in 1 sec");

        //TODO: Handle return value to be able to abort retries
        timeProvider_.EnqueueWithDelay(std::chrono::milliseconds(1000), [this]() { StartSubscribe(); });
    }
}

void IplmService::HandleMessageRcvd(const Msg& msg)
{
  ALOGD("CbLmBroadcast %04X.%04X.%02d 0x%08X(size: %d)", msg.pdu.header.serviceID, (int)msg.pdu.header.operationID,
        (int)msg.pdu.header.operationType, msg.pdu.header.seqNbr, (int)msg.pdu.payload.size());

  if (OperationType::ERROR == msg.pdu.header.operationType) return;

  first_contact = true;
  ALOGD("Got IP_Activity(%s,%s) from %d (VCM = %d, TEM = %d)", ToString(static_cast<Action>(msg.pdu.payload.data()[0])).c_str(),
        ToString(static_cast<Prio>(msg.pdu.payload.data()[1])), (int)msg.ecu, (int)Ecu::VCM, (int)Ecu::TEM);

  if (iplm_data_.action_[(int)Ecu::IHU] & ACTION_AVAILABLE)
  {
    if (msg.ecu == Ecu::VCM)
      timeProvider_.Cancel(activityVCM_);
    else
      timeProvider_.Cancel(activityTEM_);
  }

  iplm_data_.action_[(int)msg.ecu] = static_cast<Action>(msg.pdu.payload.data()[0]);
  iplm_data_.prio_[(int)msg.ecu] = static_cast<Prio>(msg.pdu.payload.data()[1]);

  iplm_data_.rg1_availabilityStatus_.set(static_cast<int>((msg.ecu == Ecu::VCM) ? EcuId::ECU_Vcm : EcuId::ECU_Tem));
  iplm_data_.rg3_availabilityStatus_.set(static_cast<int>((msg.ecu == Ecu::VCM) ? EcuId::ECU_Vcm : EcuId::ECU_Tem));

  if (flexray_wakeup_attempted && iplm_data_.rg1_availabilityStatus_.all() && iplm_data_.rg3_availabilityStatus_.all()) {
    flexray_wakeup_attempted = false;
  }
  // Handle session state here?

  if (iplm_data_.action_[(int)Ecu::IHU] & ACTION_AVAILABLE)
  {
    if (msg.ecu == Ecu::VCM)
      restartVcmActivityTimer();
    else
      restartTemActivityTimer();
  }
}

// Methods from vendor::volvocars::hardware::vehiclecom::V1_0::IMessageCallback follow.
Return<void> IplmService::onMessageRcvd(const Msg& msg)
{
    IDispatcher::EnqueueTask([msg, this]() {
        HandleMessageRcvd(msg);
    });

    return Void();
}

bool IplmService::Initialize()
{
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
    /*registerNotificationCyclicCallback(VccIpCmd::ServiceId::LM,
                                       VccIpCmd::OperationId::IpActivity,
                                       std::bind(&IpLmServiceStubImpl::CbLmBroadcast, this, std::placeholders::_1));
*/
    restartTemActivityTimer();
    restartVcmActivityTimer();

    // A recurrent timer used to send IHU's IP activity messages
    // Timeout as specified in REQPROD 347839
    activityTimer_ = timeProvider_.EnqueueWithDelay(std::chrono::milliseconds(1000), [this]() { ActivityTimeout(); });

    return true;
}

void IplmService::FlexrayWakeupTimeout()
{
    autosar::NetActvtRec1 wakeup;
    wakeup.ResourceGroup = 0;
    wakeup.Prio = autosar::PrioHighNormal::PrioNormal;

    senderWakeup_.send(wakeup);

    ALOGI("FlexrayWakeupTimeout");
}

bool IplmService::SendFlexrayWakeup(ResourceGroup _rg, Prio _prio)
{
    // If any LSC locally has requested RG to be kept active and RG is unavailable; time to request wakeup
    if (((iplm_data_.action_[(int)Ecu::IHU] & ACTION_AVAILABLE) != ACTION_AVAILABLE) ||
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
    autosar::NetActvtRec1 wakeup;
    wakeup.ResourceGroup = _rg;
    wakeup.Prio = _prio == PRIO_HIGH ? autosar::PrioHighNormal::PrioHigh : autosar::PrioHighNormal::PrioNormal;

    auto tmoHandler = std::bind(&IplmService::FlexrayWakeupTimeout, this);
    senderWakeup_.send(wakeup);

    // Timeout as specified in REQPROD 347896
    ApplicationDataElement::DESender<ActivateVfc_info> activatevfc;
    activatevfc.send({Vfc::IPWakeup, 3});
    flexray_wakeup_attempted = true;

    senderWakeupTimer_ = timeProvider_.EnqueueWithDelay(std::chrono::milliseconds(3000), tmoHandler);
    ALOGI("SendFlexrayWakeup rg=%d , prio=%d",_rg,_prio);
    return true;
}

void IplmService::restartVcmActivityTimer()
{
    // RequestMonitoringTimeout is configurable through Local config as required by REQPROD 347880
    activityVCM_ = timeProvider_.EnqueueWithDelay(std::chrono::milliseconds(LocalCfg::getRequestMonitoringTmo() + 100),
                                                      [this]() { RequestMonitoringTimeout(EcuId::ECU_Vcm); });
}
void IplmService::restartTemActivityTimer()
{
    // RequestMonitoringTimeout is configurable through Local config as required by REQPROD 347880
    activityTEM_ = timeProvider_.EnqueueWithDelay(std::chrono::milliseconds(LocalCfg::getRequestMonitoringTmo() + 100),
                                                      [this]() { RequestMonitoringTimeout(EcuId::ECU_Tem); });
}

void IplmService::ActivityTimeout()
{
    ALOGD("+ ActivityTimeout");

    bool broadcast_allowed = false;
    bool tem_available = false;
    bool vcm_available = false;
    auto rg1_status = XResourceGroupStatus::Available;

    broadcast_allowed =
        (LocalCfg::getNofLocalSoftwareComponents() <= static_cast<int>(iplm_data_.registered_LSCs_.size()));

    rg1_status = iplm_data_.rg1_availabilityStatus_.all()
                     ? XResourceGroupStatus::Available
                     : iplm_data_.rg1_availabilityStatus_.any()
                           ? XResourceGroupStatus::PartlyAvailable
                           : XResourceGroupStatus::Unavailable;
    tem_available = iplm_data_.rg1_availabilityStatus_.test(static_cast<int>(EcuId::ECU_Tem));
    vcm_available = iplm_data_.rg1_availabilityStatus_.test(static_cast<int>(EcuId::ECU_Vcm));

    if (broadcast_allowed)
    {
        ALOGD("ActivityTimeout: Sending IP Activity broadcast");

        // time to send new activity message
        CreateAndSendIpActivityMessage();
    }
    else
    {
        ALOGD("ActivityTimeout: No LSCs registered, not sending IP Activity broadcast");
    }

    // send events to LSCs. Update is sent periodically every second
    for (auto regs : iplm_data_.registered_callbacks_) {
        auto callback = regs.second;
        auto result1 = callback->onResourceGroupStatus(XResourceGroup::ResourceGroup1, rg1_status);
        auto result2 = callback->onResourceGroupStatus(XResourceGroup::ResourceGroup3, rg1_status);
        auto result3 = callback->onNodeStatus(Ecu::TEM, tem_available);
        auto result4 = callback->onNodeStatus(Ecu::VCM, vcm_available);
        // We always have to call isOk() eventthough we don't need it
        // https://source.android.com/devices/architecture/hidl-cpp/functions
        result1.isOk();
        result2.isOk();
        result3.isOk();
        result4.isOk();
        bool isDead = result1.isDeadObject() || result2.isDeadObject() || result3.isDeadObject() || result4.isDeadObject();
        if (isDead) {
            ALOGI("IplmService::ActivityTimeout, %s is dead, lets remove it",regs.first.c_str());
            unregisterService(regs.first);
        }
    }
    //ALOGI("Tem available: %d",tem_available);
    //ALOGI("Vcm available: %d",vcm_available);

    for (auto const& notifyAvailabilityToRegisteredService : node_availability_notifications_)
    {
        notifyAvailabilityToRegisteredService(EcuId::ECU_Tem, tem_available);
        notifyAvailabilityToRegisteredService(EcuId::ECU_Vcm, vcm_available);
    }

    //Restart activity timer
    activityTimer_ = timeProvider_.EnqueueWithDelay(std::chrono::milliseconds(1000), [this]() { ActivityTimeout(); });

    ALOGD("- ActivityTimeout");
}

void IplmService::RequestMonitoringTimeout(EcuId ecu)
{
    // No IP Activity message received from remote node in last request_monitoring_time. Node died may be?

    first_contact = true;

    ALOGD("Timeout on IP_Activity for %d", ecu);

    iplm_data_.rg1_availabilityStatus_.reset(static_cast<int>(ecu));
    iplm_data_.rg3_availabilityStatus_.reset(static_cast<int>(ecu));

    // Node not available. Reset AVAILABLE bit in action_ corresponding to ECU
    (ecu == EcuId::ECU_Vcm) ? (iplm_data_.action_[static_cast<int>((int)Ecu::VCM)] = 0)
                            : (iplm_data_.action_[static_cast<int>((int)Ecu::TEM)] = 0);

    SendFlexrayWakeup(ResourceGroup::RG_1, iplm_data_.prio_[(int)Ecu::IHU]);
}

void IplmService::CreateAndSendIpActivityMessage()
{
    ALOGD("LM: Timeout, prepare new broadcast message");

    Action action = iplm_data_.action_[(int)Ecu::IHU];
    Prio prio = iplm_data_.prio_[(int)Ecu::IHU];

    // Activity messages shall be sent only when ACTION_AVAILABLE bit is set. REQPROD 347878
    if ((action & ACTION_AVAILABLE) != ACTION_AVAILABLE)
        return;

    Msg message;

    message.ecu = Ecu::ALL;

    //TODO: Make less hard coded!!
    // Prepare header. REQPROD:347119
    message.pdu.header.serviceID = 0xFFFF;
    message.pdu.header.operationID = 0xFF01;
    message.pdu.header.operationType = OperationType::NOTIFICATION_CYCLIC;
    //message.pdu.header.encoding = VccIpCmd::DataType:NOT_ENCODED;
    message.pdu.header.seqNbr = sequenceId_++;

    message.pdu.payload = std::vector<uint8_t>({action, (uint8_t)prio, 0, 0});

    ALOGD("Send IP_Activity(%s,%s)", ToString(action).c_str(), ToString(prio));

    CommandResult result;
    ipcbServer_.get()->sendMessage(message, {false, 0, 0}, [&result](CommandResult sr) { result = sr; });
    if (!result.success)
    {
        ALOGE("sendMessage failed with error: %s", result.errMsg.c_str());
    }
}

bool IplmService::IsRgRequestedLocally(const IplmData& iplm_data, const ResourceGroup rg, const Prio prio)
{
    return std::any_of(iplm_data.registered_LSCs_.cbegin(), iplm_data.registered_LSCs_.cend(),
                     [rg, prio](const ServicePrioMap::value_type& item) {
                       if (item.second.rg_ & ResourceGroup::RG_1)
                       {
                         return item.second.prio_rg1_ == prio;
                       }
                       else if (item.second.rg_ & ResourceGroup::RG_3)
                       {
                         return item.second.prio_rg3_ == prio;
                       }
                       else
                       {
                         return false;
                       }
                     });
}

bool IplmService::IsRgRequestedLocally(const IplmData& iplm_data, const ResourceGroup rg)
{
    return std::any_of(iplm_data.registered_LSCs_.cbegin(), iplm_data.registered_LSCs_.cend(),
                       [rg](const ServicePrioMap::value_type& item) { return (item.second.rg_ & rg); });
}

bool IplmService::IsServiceRegistered(const IplmData& iplm_data, const std::string& lscName)
{
    return iplm_data.registered_LSCs_.find(lscName) != iplm_data.registered_LSCs_.end();
}

bool IplmService::IsServiceRegistered(IplmData& iplm_data, const std::string& lscName, ServicePrioMap::iterator& it)
{
    it = iplm_data.registered_LSCs_.find(lscName);
    if (it == iplm_data.registered_LSCs_.end())
    {
        return false;
    }
    return true;
}

bool IplmService::IsRequestedRGValid(XResourceGroup rg)
{
    return XResourceGroup::ResourceGroup1 == rg || XResourceGroup::ResourceGroup3 == rg;
}

bool IplmService::IsRgRequestedByExternalNode(const IplmData &iplm_data, const ResourceGroup rg)
{
    return (iplm_data.action_[(int)Ecu::TEM] & rg) || (iplm_data.action_[(int)Ecu::VCM] & rg);
}

bool IplmService::GetExternalPrio(const IplmData &iplm_data)
{
    return iplm_data.prio_[(int)Ecu::VCM] == PRIO_HIGH || iplm_data.prio_[(int)Ecu::TEM] == PRIO_HIGH;
}

//
// Helper functions for friendly debug output
//

const char* IplmService::ToString(const ResourceGroup r)
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

const char* IplmService::ToString(const XResourceGroup r)
{
    switch (r)
    {
      case XResourceGroup::ResourceGroup1:
        return "RG_1";
      case XResourceGroup::ResourceGroup3:
        return "RG_3";
      default:
        ALOGW("ToString() called with unsupported ResourceGroup");
        return "";
  }
}

const std::string IplmService::ToString(const Action a)
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

const char* IplmService::ToString(const Prio a)
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

const char* IplmService::ToString(const XResourceGroupPrio prio)
{
    switch (prio)
    {
      case XResourceGroupPrio::Normal:
          return "NORM";
      case XResourceGroupPrio::High:
          return "HIGH";
      default:
          ALOGW("ToString() called with unsupported Prio");
          return "";
    }
}

Return<bool> IplmService::requestResourceGroup(const hidl_string& lscName, XResourceGroup _rg, XResourceGroupPrio _prio)
{
    ALOGI("Request called for service (%s) and RG (%s) and prio (%s)",
        lscName.c_str(),
        ToString(_rg),
        ToString(_prio));

    // IHU is part of RG1 or RG3. Filter out requests for other RGs
    if (!IsRequestedRGValid(_rg))
    {
        ALOGW("unexpected resource group %hhu", _rg);
        return false;
    }

    const ResourceGroup rg = (XResourceGroup::ResourceGroup1 == _rg) ? ResourceGroup::RG_1 : ResourceGroup::RG_3;
    const Prio prio = (XResourceGroupPrio::Normal == _prio) ? PRIO_NORM : PRIO_HIGH;

    // Look for registered service and update requested parameters corresponding to service
    ServicePrioMap::iterator it;
    if (!IsServiceRegistered(iplm_data_, lscName, it))
    {
        ALOGW("RG request from unregistered service %s",lscName.c_str());
        return false;
    }

    // update requested rg in per service record
    it->second.rg_ = static_cast<ResourceGroup>(rg | it->second.rg_);
    // update requested prio in per service record
    if (rg & ResourceGroup::RG_1)
    {
        it->second.prio_rg1_ = prio;
    }
    else
    {
        it->second.prio_rg3_ = prio;
    }

    // Set requested RG in action_ so that it is included in activity messages sent by IHU
    iplm_data_.action_[(int)Ecu::IHU] |= rg;
    if (prio == PRIO_HIGH)
    {
        iplm_data_.prio_[(int)Ecu::IHU] = PRIO_HIGH;
    }
    else if (!IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_1, PRIO_HIGH) &&
             !IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_3, PRIO_HIGH))
    {
        iplm_data_.prio_[(int)Ecu::IHU] = PRIO_NORM;
    }

    SetNsmSessionState();
    SendFlexrayWakeup(rg, prio);

    return true;
}

Return<bool> IplmService::releaseResourceGroup(const hidl_string& lscName, XResourceGroup _rg)
{
    ALOGI("Release called for service (%s) and RG (%s)", lscName.c_str(), ToString(_rg));

    // IHU is part of RG1 or RG3. Filter out requests for other RGs
    if (!IsRequestedRGValid(_rg))
    {
        ALOGW("Unexpected resource group %hhu. Ignored", _rg);
        return false;
    }

    const ResourceGroup rg = (XResourceGroup::ResourceGroup1 == _rg) ? ResourceGroup::RG_1 : ResourceGroup::RG_3;

    ServicePrioMap::iterator it;
    if (!IsServiceRegistered(iplm_data_, lscName, it))
    {
        ALOGW("RG request from unregistered service (%s). Ignored",lscName.c_str());
        return false;
    }

    // Reset corresponding RG bit for the LSC
    it->second.rg_ = static_cast<ResourceGroup>(it->second.rg_ & (~rg));
    // Reset prio for the LSC
    if (rg & ResourceGroup::RG_1)
    {
        it->second.prio_rg1_ = PRIO_NORM;
    }
    else
    {
        it->second.prio_rg3_ = PRIO_NORM;
    }

    // If no LSC is requesting high prio for any RG locally; reset HIGH PRIO for IHU
    if (!IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_1, PRIO_HIGH) &&
        !IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_3, PRIO_HIGH))
    {
        iplm_data_.prio_[(int)Ecu::IHU] = PRIO_NORM;
        SetNsmSessionState();
    }

    // Check if any LSC is still interested in this particular resource_group
    if (!IsRgRequestedLocally(iplm_data_, rg))
    {
        // as no other LSC is requesting this RG; reset corresponding bit
        iplm_data_.action_[(int)Ecu::IHU] &= (~rg);
    }

    return true;
}

Return<bool> IplmService::registerService(const hidl_string& lscName, const sp<IIplmCallback>& iIplmCallback)
{
    ALOGI("RegisterService: called for service (%s)", lscName.c_str());

    // register a new service
    if (!IsServiceRegistered(iplm_data_, lscName))
    {
        // Insert new service to registered LSCs list
        iplm_data_.registered_LSCs_[lscName] = RGRequestInfo();
        iplm_data_.registered_callbacks_[lscName] = iIplmCallback;

        // If all LSCs are registered; mark ACTION_AVAILABLE to start IP_activity broadcast. Ref: REQPROD 347878
        if (LocalCfg::getNofLocalSoftwareComponents() <= static_cast<int>(iplm_data_.registered_LSCs_.size()))
        {
            iplm_data_.action_[(int)Ecu::IHU] |= ACTION_AVAILABLE;

            // As Local IPLinkManager is now available; we should start monitoring remote nodes for activity
            restartVcmActivityTimer();
            restartTemActivityTimer();
        }
    }
    else
    {
        ALOGW("registration request from already registered service (%s) ", lscName.c_str());
    }

    return true;
}

Return<bool> IplmService::unregisterService(const hidl_string& lscName)
{
    ALOGI("UnRegisterService: called for service (%s)", lscName.c_str());

    ServicePrioMap::iterator it;
    if (!IsServiceRegistered(iplm_data_, lscName, it))
    {
        ALOGW("UnRegistration request from a not registered service (%s)", lscName.c_str());
        return false;
    }

    iplm_data_.registered_LSCs_.erase(it);
    iplm_data_.registered_callbacks_.erase(lscName);

    // If all LSCs are not registered; reset ACTION_AVAILABLE to STOP IP_activity broadcast. Ref: REQPROD 347878
    if (LocalCfg::getNofLocalSoftwareComponents() > static_cast<int>(iplm_data_.registered_LSCs_.size()))
    {
        iplm_data_.action_[(int)Ecu::IHU] |= (~ACTION_AVAILABLE);
    }

    if (!IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_1, PRIO_HIGH) &&
        !IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_3, PRIO_HIGH))
    {
        // No LSC needs HIGH prio on RG1 or RG3
        iplm_data_.prio_[(int)Ecu::IHU] = PRIO_NORM;
        SetNsmSessionState();
    }

    if (!IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_1))
    {
        // reset corresponding RG bit as no other LSCs wants this RG
        iplm_data_.action_[(int)Ecu::IHU] &= (~ResourceGroup::RG_1);
    }

    if (!IsRgRequestedLocally(iplm_data_, ResourceGroup::RG_3))
    {
        // reset corresponding RG bit as no other LSCs wants this RG
        iplm_data_.action_[(int)Ecu::IHU] &= (~ResourceGroup::RG_3);
    }

    return true;
}

// Reference: https://delphisweden.atlassian.net/wiki/display/VI/NSM+-+Session+-+IPLM+Resource+Groups
bool IplmService::SetNsmSessionState()
{
    // TODO figure out what this is doing and how we can use our new shutdown prevention API instead
    if ((iplm_data_.action_[(int)Ecu::IHU] & ACTION_AVAILABLE) != ACTION_AVAILABLE || !first_contact)
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
