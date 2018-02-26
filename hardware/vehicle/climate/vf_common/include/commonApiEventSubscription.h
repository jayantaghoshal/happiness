/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#pragma once
#include <CommonAPI/CommonAPI.hpp>

namespace VehicleFunctions
{

class ICommonApiEventSubscription
{
public:
    virtual ~ICommonApiEventSubscription()
    {
    }
};

template <typename ... TEventArgs>
class CommonApiEventSubscription : public ICommonApiEventSubscription
{
private:
    CommonAPI::Event<TEventArgs...>&                             e;
    const typename CommonAPI::Event<TEventArgs...>::Subscription subscriptionHandle;

public:
    explicit CommonApiEventSubscription(CommonAPI::Event<TEventArgs...>& e, typename CommonAPI::Event<TEventArgs...>::Listener& listener)
        : ICommonApiEventSubscription()
        , e(e)
        , subscriptionHandle(e.subscribe(listener))
    {
    }
    virtual ~CommonApiEventSubscription()
    {
        e.unsubscribe(subscriptionHandle);
    }

    CommonApiEventSubscription(const CommonApiEventSubscription&) = delete;
    CommonApiEventSubscription(CommonApiEventSubscription&&)      = delete;
    CommonApiEventSubscription& operator=(const CommonApiEventSubscription&) = delete;
    CommonApiEventSubscription& operator=(CommonApiEventSubscription&&) = delete;
};

// helper function because constructors cant have template argument deduction
template <typename ... TEventArgs>
std::unique_ptr<ICommonApiEventSubscription>
make_commonapi_subscription(CommonAPI::Event<TEventArgs...>& e, typename CommonAPI::Event<TEventArgs...>::Listener listener)
{
    return std::unique_ptr<ICommonApiEventSubscription>(new CommonApiEventSubscription<TEventArgs...>(e, listener));
}
}
