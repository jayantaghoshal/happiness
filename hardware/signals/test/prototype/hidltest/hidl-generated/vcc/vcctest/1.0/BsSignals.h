#ifndef HIDL_GENERATED_VCC_VCCTEST_V1_0_BSSIGNALS_H
#define HIDL_GENERATED_VCC_VCCTEST_V1_0_BSSIGNALS_H

#include <cutils/trace.h>
#include <future>
#include <vcc/vcctest/1.0/ISignals.h>

#include <hidl/HidlPassthroughSupport.h>
#include <hidl/TaskRunner.h>
namespace vcc {
namespace vcctest {
namespace V1_0 {

struct BsSignals : ISignals, ::android::hardware::details::HidlInstrumentor {
    explicit BsSignals(const ::android::sp<ISignals> impl);
    // Methods from ISignals follow.
    ::android::hardware::Return<uint32_t> set(const ::android::hardware::hidl_string& propValue) {
        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignals::set::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&propValue);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignals", "set", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->set(propValue);

        uint32_t _hidl_out_status = _hidl_return;
        atrace_end(ATRACE_TAG_HAL);
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&_hidl_out_status);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignals", "set", &_hidl_args);
            }
        }

        return _hidl_return;
    }
    ::android::hardware::Return<void> subscribe(const ::android::sp<ISignalsCallback>& cb) {
        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignals::subscribe::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&cb);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignals", "subscribe", &_hidl_args);
            }
        }

        ::android::sp<ISignalsCallback> _hidl_wrapped_cb;
        if (cb != nullptr && !cb->isRemote()) {
            _hidl_wrapped_cb = ISignalsCallback::castFrom(::android::hardware::details::wrapPassthrough<ISignalsCallback>(cb));
            if (_hidl_wrapped_cb == nullptr) {
                return ::android::hardware::Status::fromExceptionCode(
                        ::android::hardware::Status::EX_TRANSACTION_FAILED,
                        "Cannot wrap passthrough interface.");
            }
        } else {
            _hidl_wrapped_cb = cb;
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->subscribe(_hidl_wrapped_cb);

        atrace_end(ATRACE_TAG_HAL);
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignals", "subscribe", &_hidl_args);
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

        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignals::interfaceChain::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignals", "interfaceChain", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->interfaceChain([&](const auto &_hidl_out_descriptors) {
            atrace_end(ATRACE_TAG_HAL);
            if (UNLIKELY(mEnableInstrumentation)) {
                std::vector<void *> _hidl_args;
                _hidl_args.push_back((void *)&_hidl_out_descriptors);
                for (const auto &callback: mInstrumentationCallbacks) {
                    callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignals", "interfaceChain", &_hidl_args);
                }
            }

            _hidl_cb(_hidl_out_descriptors);
        });

        return _hidl_return;
    }
    ::android::hardware::Return<void> debug(const ::android::hardware::hidl_handle& fd, const ::android::hardware::hidl_vec<::android::hardware::hidl_string>& options) {
        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignals::debug::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&fd);
            _hidl_args.push_back((void *)&options);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignals", "debug", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->debug(fd, options);

        atrace_end(ATRACE_TAG_HAL);
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignals", "debug", &_hidl_args);
            }
        }

        return _hidl_return;
    }
    ::android::hardware::Return<void> interfaceDescriptor(interfaceDescriptor_cb _hidl_cb) {
        if (_hidl_cb == nullptr) {
            return ::android::hardware::Status::fromExceptionCode(
                    ::android::hardware::Status::EX_ILLEGAL_ARGUMENT);
        }

        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignals::interfaceDescriptor::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignals", "interfaceDescriptor", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->interfaceDescriptor([&](const auto &_hidl_out_descriptor) {
            atrace_end(ATRACE_TAG_HAL);
            if (UNLIKELY(mEnableInstrumentation)) {
                std::vector<void *> _hidl_args;
                _hidl_args.push_back((void *)&_hidl_out_descriptor);
                for (const auto &callback: mInstrumentationCallbacks) {
                    callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignals", "interfaceDescriptor", &_hidl_args);
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

        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignals::getHashChain::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignals", "getHashChain", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->getHashChain([&](const auto &_hidl_out_hashchain) {
            atrace_end(ATRACE_TAG_HAL);
            if (UNLIKELY(mEnableInstrumentation)) {
                std::vector<void *> _hidl_args;
                _hidl_args.push_back((void *)&_hidl_out_hashchain);
                for (const auto &callback: mInstrumentationCallbacks) {
                    callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignals", "getHashChain", &_hidl_args);
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
        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignals::linkToDeath::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&recipient);
            _hidl_args.push_back((void *)&cookie);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignals", "linkToDeath", &_hidl_args);
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
                callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignals", "linkToDeath", &_hidl_args);
            }
        }

        return _hidl_return;
    }
    ::android::hardware::Return<void> ping() {
        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignals::ping::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignals", "ping", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->ping();

        atrace_end(ATRACE_TAG_HAL);
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignals", "ping", &_hidl_args);
            }
        }

        return _hidl_return;
    }
    ::android::hardware::Return<void> getDebugInfo(getDebugInfo_cb _hidl_cb) {
        if (_hidl_cb == nullptr) {
            return ::android::hardware::Status::fromExceptionCode(
                    ::android::hardware::Status::EX_ILLEGAL_ARGUMENT);
        }

        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignals::getDebugInfo::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignals", "getDebugInfo", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = mImpl->getDebugInfo([&](const auto &_hidl_out_info) {
            atrace_end(ATRACE_TAG_HAL);
            if (UNLIKELY(mEnableInstrumentation)) {
                std::vector<void *> _hidl_args;
                _hidl_args.push_back((void *)&_hidl_out_info);
                for (const auto &callback: mInstrumentationCallbacks) {
                    callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignals", "getDebugInfo", &_hidl_args);
                }
            }

            _hidl_cb(_hidl_out_info);
        });

        return _hidl_return;
    }
    ::android::hardware::Return<void> notifySyspropsChanged() {
        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignals::notifySyspropsChanged::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignals", "notifySyspropsChanged", &_hidl_args);
            }
        }

        auto _hidl_error = ::android::hardware::Void();
        auto _hidl_return = addOnewayTask([mImpl = this->mImpl, mEnableInstrumentation = this->mEnableInstrumentation, mInstrumentationCallbacks = this->mInstrumentationCallbacks, &_hidl_error] {
            mImpl->notifySyspropsChanged();

            atrace_end(ATRACE_TAG_HAL);
            if (UNLIKELY(mEnableInstrumentation)) {
                std::vector<void *> _hidl_args;
                for (const auto &callback: mInstrumentationCallbacks) {
                    callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignals", "notifySyspropsChanged", &_hidl_args);
                }
            }

        });
        return _hidl_return;
    }
    ::android::hardware::Return<bool> unlinkToDeath(const ::android::sp<::android::hardware::hidl_death_recipient>& recipient) {
        atrace_begin(ATRACE_TAG_HAL, "HIDL::ISignals::unlinkToDeath::passthrough");
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&recipient);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::PASSTHROUGH_ENTRY, "vcc.vcctest", "1.0", "ISignals", "unlinkToDeath", &_hidl_args);
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
                callback(InstrumentationEvent::PASSTHROUGH_EXIT, "vcc.vcctest", "1.0", "ISignals", "unlinkToDeath", &_hidl_args);
            }
        }

        return _hidl_return;
    }

private:
    const ::android::sp<ISignals> mImpl;
    ::android::hardware::details::TaskRunner mOnewayQueue;

    ::android::hardware::Return<void> addOnewayTask(std::function<void(void)>);

};

}  // namespace V1_0
}  // namespace vcctest
}  // namespace vcc

#endif  // HIDL_GENERATED_VCC_VCCTEST_V1_0_BSSIGNALS_H
