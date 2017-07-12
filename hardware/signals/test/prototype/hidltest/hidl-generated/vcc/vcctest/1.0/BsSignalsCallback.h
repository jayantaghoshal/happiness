#ifndef HIDL_GENERATED_VCC_VCCTEST_V1_0_BSSIGNALSCALLBACK_H
#define HIDL_GENERATED_VCC_VCCTEST_V1_0_BSSIGNALSCALLBACK_H

#include <cutils/trace.h>
#include <future>
#include <vcc/vcctest/1.0/ISignalsCallback.h>

#include <hidl/HidlPassthroughSupport.h>
#include <hidl/TaskRunner.h>
namespace vcc {
namespace vcctest {
namespace V1_0 {

struct BsSignalsCallback : ISignalsCallback, ::android::hardware::details::HidlInstrumentor {
    explicit BsSignalsCallback(const ::android::sp<ISignalsCallback> impl);
    // Methods from ISignalsCallback follow.
    ::android::hardware::Return<void> signalChanged(const ::android::hardware::hidl_string& signalName) {
        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignalsCallback::signalChanged::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&signalName);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignalsCallback", "signalChanged", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->signalChanged(signalName);

        atrace_end(ATRACE_TAG_HAL);
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignalsCallback", "signalChanged", &_hidl_args);
            }
        }

        return _hidl_return;
    }

    // Methods from ::android::hidl::base::V1_0::IBase follow.
    ::android::hardware::Return<void> interfaceChain(interfaceChain_cb _hidl_cb) {
        if (_hidl_cb == nullptr) {
            return ::android::hardware::Status::fromExceptionCode(
                    ::android::hardware::Status::EX_ILLEGAL_ARGUMENT);
        }

        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignalsCallback::interfaceChain::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignalsCallback", "interfaceChain", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->interfaceChain([&](const auto &_hidl_out_descriptors) {
            atrace_end(ATRACE_TAG_HAL);
            if (UNLIKELY(mEnableInstrumentation)) {
                std::vector<void *> _hidl_args;
                _hidl_args.push_back((void *)&_hidl_out_descriptors);
                for (const auto &callback: mInstrumentationCallbacks) {
                    callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignalsCallback", "interfaceChain", &_hidl_args);
                }
            }

            _hidl_cb(_hidl_out_descriptors);
        });

        return _hidl_return;
    }
    ::android::hardware::Return<void> debug(const ::android::hardware::hidl_handle& fd, const ::android::hardware::hidl_vec<::android::hardware::hidl_string>& options) {
        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignalsCallback::debug::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&fd);
            _hidl_args.push_back((void *)&options);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignalsCallback", "debug", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->debug(fd, options);

        atrace_end(ATRACE_TAG_HAL);
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignalsCallback", "debug", &_hidl_args);
            }
        }

        return _hidl_return;
    }
    ::android::hardware::Return<void> interfaceDescriptor(interfaceDescriptor_cb _hidl_cb) {
        if (_hidl_cb == nullptr) {
            return ::android::hardware::Status::fromExceptionCode(
                    ::android::hardware::Status::EX_ILLEGAL_ARGUMENT);
        }

        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignalsCallback::interfaceDescriptor::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignalsCallback", "interfaceDescriptor", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->interfaceDescriptor([&](const auto &_hidl_out_descriptor) {
            atrace_end(ATRACE_TAG_HAL);
            if (UNLIKELY(mEnableInstrumentation)) {
                std::vector<void *> _hidl_args;
                _hidl_args.push_back((void *)&_hidl_out_descriptor);
                for (const auto &callback: mInstrumentationCallbacks) {
                    callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignalsCallback", "interfaceDescriptor", &_hidl_args);
                }
            }

            _hidl_cb(_hidl_out_descriptor);
        });

        return _hidl_return;
    }
    ::android::hardware::Return<void> getHashChain(getHashChain_cb _hidl_cb) {
        if (_hidl_cb == nullptr) {
            return ::android::hardware::Status::fromExceptionCode(
                    ::android::hardware::Status::EX_ILLEGAL_ARGUMENT);
        }

        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignalsCallback::getHashChain::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignalsCallback", "getHashChain", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->getHashChain([&](const auto &_hidl_out_hashchain) {
            atrace_end(ATRACE_TAG_HAL);
            if (UNLIKELY(mEnableInstrumentation)) {
                std::vector<void *> _hidl_args;
                _hidl_args.push_back((void *)&_hidl_out_hashchain);
                for (const auto &callback: mInstrumentationCallbacks) {
                    callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignalsCallback", "getHashChain", &_hidl_args);
                }
            }

            _hidl_cb(_hidl_out_hashchain);
        });

        return _hidl_return;
    }
    ::android::hardware::Return<void> setHALInstrumentation() {
        configureInstrumentation();
        return ::android::hardware::Void();
    }

    ::android::hardware::Return<bool> linkToDeath(const ::android::sp<::android::hardware::hidl_death_recipient>& recipient, uint64_t cookie) {
        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignalsCallback::linkToDeath::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&recipient);
            _hidl_args.push_back((void *)&cookie);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignalsCallback", "linkToDeath", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->linkToDeath(recipient, cookie);

        bool _hidl_out_success = _hidl_return;
        atrace_end(ATRACE_TAG_HAL);
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&_hidl_out_success);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignalsCallback", "linkToDeath", &_hidl_args);
            }
        }

        return _hidl_return;
    }
    ::android::hardware::Return<void> ping() {
        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignalsCallback::ping::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignalsCallback", "ping", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->ping();

        atrace_end(ATRACE_TAG_HAL);
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignalsCallback", "ping", &_hidl_args);
            }
        }

        return _hidl_return;
    }
    ::android::hardware::Return<void> getDebugInfo(getDebugInfo_cb _hidl_cb) {
        if (_hidl_cb == nullptr) {
            return ::android::hardware::Status::fromExceptionCode(
                    ::android::hardware::Status::EX_ILLEGAL_ARGUMENT);
        }

        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignalsCallback::getDebugInfo::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignalsCallback", "getDebugInfo", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->getDebugInfo([&](const auto &_hidl_out_info) {
            atrace_end(ATRACE_TAG_HAL);
            if (UNLIKELY(mEnableInstrumentation)) {
                std::vector<void *> _hidl_args;
                _hidl_args.push_back((void *)&_hidl_out_info);
                for (const auto &callback: mInstrumentationCallbacks) {
                    callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignalsCallback", "getDebugInfo", &_hidl_args);
                }
            }

            _hidl_cb(_hidl_out_info);
        });

        return _hidl_return;
    }
    ::android::hardware::Return<void> notifySyspropsChanged() {
        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignalsCallback::notifySyspropsChanged::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignalsCallback", "notifySyspropsChanged", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = addOnewayTask([mImpl = this->mImpl, mEnableInstrumentation = this->mEnableInstrumentation, mInstrumentationCallbacks = this->mInstrumentationCallbacks, &_hidl_error] {
            mImpl->notifySyspropsChanged();

            atrace_end(ATRACE_TAG_HAL);
            if (UNLIKELY(mEnableInstrumentation)) {
                std::vector<void *> _hidl_args;
                for (const auto &callback: mInstrumentationCallbacks) {
                    callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignalsCallback", "notifySyspropsChanged", &_hidl_args);
                }
            }

        });
        return _hidl_return;
    }
    ::android::hardware::Return<bool> unlinkToDeath(const ::android::sp<::android::hardware::hidl_death_recipient>& recipient) {
        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignalsCallback::unlinkToDeath::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&recipient);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignalsCallback", "unlinkToDeath", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->unlinkToDeath(recipient);

        bool _hidl_out_success = _hidl_return;
        atrace_end(ATRACE_TAG_HAL);
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&_hidl_out_success);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignalsCallback", "unlinkToDeath", &_hidl_args);
            }
        }

        return _hidl_return;
    }

private:
    const ::android::sp<ISignalsCallback> mImpl;
    ::android::hardware::details::TaskRunner mOnewayQueue;

    ::android::hardware::Return<void> addOnewayTask(std::function<void(void)>);

};

}  // namespace V1_0
}  // namespace vcctest
}  // namespace vcc

#endif  // HIDL_GENERATED_VCC_VCCTEST_V1_0_BSSIGNALSCALLBACK_H
