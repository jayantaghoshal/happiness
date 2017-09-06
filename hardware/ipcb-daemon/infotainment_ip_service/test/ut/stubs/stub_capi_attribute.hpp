/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef STUB_CAPI_ATTRIBUTE_HPP_
#define STUB_CAPI_ATTRIBUTE_HPP_

#include <CommonAPI/CommonAPI.hpp>

template <typename... Arguments_>
class MockCommonApiEvent : public CommonAPI::Event<Arguments_...>
{
public:
    void simulateNotifyListeners(const Arguments_&... _eventArguments)
    {
        this->notifyListeners(_eventArguments...);  // Calls protected method in Event
    }
};

#if !defined(COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif
// I don't know why SelectiveEvent is not included in the same way as Event.hpp (CommonAPI->Types->Event)
// Looks like a bug in CommonAPI to me, should be ok to hack it since we are mocking internal behaviour
#include <CommonAPI/SelectiveEvent.hpp>
#undef COMMONAPI_INTERNAL_COMPILATION

template <typename... Arguments_>
class MockCommonApiSelectiveEvent : public CommonAPI::SelectiveEvent<Arguments_...>
{
public:
    void simulateNotifyListeners(const Arguments_&... _eventArguments)
    {
        this->notifyListeners(_eventArguments...);  // Calls protected method in Event
    }
};

template <typename T>
class MockCommonApiAttribute : public CommonAPI::ObservableReadonlyAttribute<T>
{
public:
    MockCommonApiAttribute()
    {
    }
    virtual ~MockCommonApiAttribute()
    {
    }
    MockCommonApiAttribute(typename CommonAPI::ReadonlyAttribute<T>::ValueType v) : value(v)
    {
    }

    typename CommonAPI::ReadonlyAttribute<T>::ValueType value;

    MockCommonApiEvent<T> changedEvent;

    virtual void getValue(CommonAPI::CallStatus& _status,
                          typename CommonAPI::ReadonlyAttribute<T>::ValueType& _value,
                          const CommonAPI::CallInfo* _info = nullptr) const override
    {
        _value = value;
    }
    virtual std::future<CommonAPI::CallStatus> getValueAsync(
        typename CommonAPI::ReadonlyAttribute<T>::AttributeAsyncCallback attributeAsyncCallback,
        const CommonAPI::CallInfo* _info = nullptr) override
    {
        std::future<CommonAPI::CallStatus> status;
        return status;
    }

    virtual MockCommonApiEvent<T>& getChangedEvent() override
    {
        return changedEvent;
    }
};

#endif /* STUB_CAPI_ATTRIBUTE_HPP_ */
