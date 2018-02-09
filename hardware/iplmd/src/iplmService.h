/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <IDispatcher.h>
#include <mutex>
#include <unordered_map>

#include <Application_dataelement.h>

#include <vendor/volvocars/hardware/vehiclecom/1.0/IMessageCallback.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IVehicleCom.h>

#include <vendor/volvocars/hardware/iplm/1.0/IIplm.h>
#include <vendor/volvocars/hardware/iplm/1.0/IIplmCallback.h>

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;
using ::tarmac::eventloop::IDispatcher;

using ::vendor::volvocars::hardware::vehiclecom::V1_0::IVehicleCom;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::IMessageCallback;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::Msg;
using ::vendor::volvocars::hardware::common::V1_0::Ecu;

using ::vendor::volvocars::hardware::iplm::V1_0::IIplm;
using ::vendor::volvocars::hardware::iplm::V1_0::IIplmCallback;
using XResourceGroup = ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroup;
using XResourceGroupPrio = ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroupPrio;
using XResourceGroupStatus = ::vendor::volvocars::hardware::iplm::V1_0::ResourceGroupStatus;

using ::android::hardware::hidl_death_recipient;

class IplmService : public IIplm, public IMessageCallback, public ::android::hardware::hidl_death_recipient {
  public:
    /** Enumeration used to track remote ECU availability in resource group
     */
    enum class EcuId { ECU_Tem, ECU_Vcm, ECU_Max };

    /*! \brief Prioritization. Encoding as per REQPROD 347120 */
    enum Prio {
        PRIO_NORM = 0x00, /*!< Normal priority */
        PRIO_HIGH = 0x01  /*!< When requesting a high priority keep-alive, meaning the transmitting ECU and ECUs
                           * receiving ECUs should ignore normal power handling. */
    };

    IplmService();
    ~IplmService() = default;

    // Methods from ::vendor::volvocars::hardware::vehiclecom::V1_0::IMessageCallback follow.
    Return<void> onMessageRcvd(const Msg& msg) override;

    /**
       * Initialize IpLmService and the internal Link Manager functional block.
       * @param[in] msg_dispatcher  Message dispatcher to use with LinkManager.
       */
    // // hidl_death_recipient
    void serviceDied(uint64_t cookie, const android::wp<IBase>& who) override;

    bool Initialize();

    // Start subscribing to ipcb
    void StartSubscribe();
    /**
     * Deinitialize IpLmService.
     */
    void Uninitialize();

    /**
     * description: Request the given Resource Group
     * @param[in] lscName         Name of the service making the request
     * @param[in] _rg             Resource group being requested
     * @param[in] _prio           priority for the requested resource group
     */
    Return<bool> requestResourceGroup(const hidl_string& lscName, XResourceGroup resourceGroup,
                                      XResourceGroupPrio resourceGroupPrio) override;

    /**
     * description: Release the given Resource Group
     * @param[in] lscName         Name of the service making the request
     * @param[in] _rg             Resource group being released
     */
    Return<bool> releaseResourceGroup(const hidl_string& lscName, XResourceGroup resourceGroup) override;

    /**
     * description: Register Local Software Component with Link Manager
     * @param[in] lscName         Name of the service being registered
     */
    Return<bool> registerService(const hidl_string& lscName, const sp<IIplmCallback>& iIplmCallback) override;

    /**
     * description: Unregister Local Software Component with Link Manager
     * @param[in] lscName         Name of the service being de-registered
     */
    Return<bool> unregisterService(const hidl_string& lscName) override;

    /**
     * Get availibility for a specified node
     * @param [in]  ecu            Node to get availibility for
     * @param [out] resourceGroup  Resource group
     * @param [out] prio           Priority
     * @return true on success, false on error
     */
    // bool GetEcuAvailibility(Ecu ecu, uint8_t &resourceGroup, uint8_t &prio);

    using EcuAvailabilityNotification = std::function<void(EcuId ecu, bool availability)>;

    /**
     * Usage: Internal services can register a callback method to be notified on
     * Ecu availability to avoid registering public common api notifications internally.
     */
    void registerAvailabilityNotification(const EcuAvailabilityNotification& f) {
        node_availability_notifications_.push_back(f);
    }

  private:
    sp<IVehicleCom> ipcbServer_;

    /*! Action, bit-field of ResourceGroup(s). */
    struct IplmData;

    /*! The ResourceGroup determines which Resource Groups the local LM module is a member of. REQPROD 347120
     *  Current implementation supports only RG_1 and RG_3 which are applicable for IHU project
     */
    enum ResourceGroup {
        RG_Reserved = 0x00,  // reserved
        RG_1 = 0x02,         // RG1 consists of IHU, VCM and TEM
        RG_3 = 0x08          // same as RG1 with Infotainment Mode ON
    };

    /*! \brief Keeps a track of resource group requests and corresponding priorities for a registered LSC
     */
    class RGRequestInfo {
      public:
        RGRequestInfo() : rg_(ResourceGroup::RG_Reserved) {}
        ResourceGroup rg_;
        Prio prio_rg1_ = PRIO_NORM;
        Prio prio_rg3_ = PRIO_NORM;
    };

    using Action = std::uint8_t;
    using ServicePrioMap = std::map<std::string, RGRequestInfo>;

    /*! Constant: ACTION_AVAILABLE
     * Services on local ECU are available to use. Ref: REQPROD 347120*/
    static const Action ACTION_AVAILABLE = 0x01;

  private:
    // Handle received message
    void HandleMessageRcvd(const Msg& msg);

    /*! \brief Timer handler used to trigger a new broadcast to be sent. */
    void ActivityTimeout();

    void RequestMonitoringTimeout(EcuId ecu);

    static bool IsRgRequestedLocally(const IplmData& iplm_data, const ResourceGroup rg, const Prio prio);

    static bool IsRgRequestedLocally(const IplmData& iplm_data, const ResourceGroup rg);

    static bool IsServiceRegistered(const IplmData& iplm_data, const std::string& lscName);

    static bool IsServiceRegistered(IplmData& iplm_data, const std::string& lscName, ServicePrioMap::iterator& it);

    static bool IsRequestedRGValid(XResourceGroup rg);

    static bool IsRgRequestedByExternalNode(const IplmData& iplm_data, const ResourceGroup rg);

    static bool GetExternalPrio(const IplmData& iplm_data);

    // Helper function to produce debug friendly output for enumerations
    static const char* ToString(ResourceGroup r);
    static const std::string ToString(Action a);
    static const char* ToString(Prio p);
    static const char* ToString(XResourceGroup r);
    static const char* ToString(XResourceGroupPrio prio);

    /*! \brief Sequence ID as defined by the IP Command Bus Protocol. */
    std::uint8_t sequenceId_ = 0;

    tarmac::eventloop::IDispatcher& timeProvider_;
    /*! \brief Activity timer, trigger periodic broadcasts */
    tarmac::eventloop::IDispatcher::JobId activityTimer_;

    /*! \brief Resource Group Session Timer, one per ECU in RG. */
    tarmac::eventloop::IDispatcher::JobId activityTEM_;
    tarmac::eventloop::IDispatcher::JobId activityVCM_;

    void CreateAndSendIpActivityMessage();

    bool SetNsmSessionState();

    /** @brief FlexrayWakeupTimeout - time out callback to reset wakeup signal. On timeout prio for flexraywakeup
     * resets to normal*/
    void FlexrayWakeupTimeout();

    bool SendFlexrayWakeup(ResourceGroup _rg, Prio _prio);

    struct IplmData {
        /*! \brief Keeps record of registered local software componenets (LSC)
         */
        ServicePrioMap registered_LSCs_;
        std::map<std::string, sp<IIplmCallback>> registered_callbacks_;
        std::mutex registered_callbacks_mutex_;

        /*! \brief Keeps the last received/sent Action of all supported ECUs, self
         *  included.
         * \note Using Ecu as index for convinience. */
        std::array<Action, 8> action_ = {{
                0,                 // UNKNOWN - not used
                0,                 // ALL - not used
                ACTION_AVAILABLE,  // IHU - SELF (Last sent Action)
                0,                 // VCM - last received Prio from VCM
                0,                 // TEM - last received Prio from TEM
                0,                 // DIM - not used
                0,                 // TCAM
                0,                 // VGM
        }};

        /*! \brief Keeps the last received/sent Prio of all supported ECUs, self
         *  included.
         * \note Using Ecu as index for convinience. */
        std::array<Prio, 8> prio_ = {{
                PRIO_NORM,  // UNKNOWN - not used
                PRIO_NORM,  // ALL - not used
                PRIO_NORM,  // IHU - SELF (last sent prio)
                PRIO_NORM,  // VCM - last received Prio from VCM
                PRIO_NORM,  // TEM - last received Prio from TEM
                PRIO_NORM,  // DIM - not used
                PRIO_NORM,  // TCAM
                PRIO_NORM,  // VGM
        }};

        std::bitset<static_cast<int>(EcuId::ECU_Max)> rg1_availabilityStatus_;
        std::bitset<static_cast<int>(EcuId::ECU_Max)> rg3_availabilityStatus_;
    };

    IplmData iplm_data_;

    ApplicationDataElement::DESender<autosar::NetHdActvt_info> senderWakeup_;
    tarmac::eventloop::IDispatcher::JobId senderWakeupTimer_;

    void restartVcmActivityTimer();
    void restartTemActivityTimer();

    std::vector<EcuAvailabilityNotification> node_availability_notifications_;
};
