/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef MOCK_COMMONAPI_RUNTIME_HPP_
#define MOCK_COMMONAPI_RUNTIME_HPP_

#include <functional>
#include <memory>

#if !defined(COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/MainLoopContext.hpp>
#include <CommonAPI/Proxy.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace CommonAPI
{
struct MockRuntime
{
    bool available;
    std::shared_ptr<CommonAPI::Proxy> proxyDelegate;

    /**
     * Add a delegate expectation to the bottom of the stack.
     * @param delegate
     */
    void AddDelegateExpectation(std::shared_ptr<CommonAPI::Proxy> delegate)
    {
        proxyDelegate = delegate;
    }

    static std::shared_ptr<MockRuntime> get()
    {
        static std::shared_ptr<MockRuntime> r = std::make_shared<MockRuntime>();
        return r;
    }

    template <template <typename...> class _ProxyClass, typename... _AttributeExtensions>
    std::shared_ptr<_ProxyClass<_AttributeExtensions...>> buildProxy(const std::string &_domain,
                                                                     const std::string &_instance,
                                                                     const std::string &_connectionId)
    {
        (void)_domain;
        (void)_instance;
        (void)_connectionId;
        if (available)
        {
            if (proxyDelegate)
            {
                std::shared_ptr<_ProxyClass<_AttributeExtensions...>> proxy =
                    std::make_shared<_ProxyClass<_AttributeExtensions...>>(proxyDelegate);
                return proxy;
            }
            else
            {
                std::shared_ptr<_ProxyClass<_AttributeExtensions...>> proxy =
                    std::make_shared<_ProxyClass<_AttributeExtensions...>>(nullptr);
                return proxy;
            }
        }
        std::cout << "Non-available nullptr" << std::endl;
        return nullptr;
    }

    template <template <typename...> class _ProxyClass, typename... _AttributeExtensions>
    std::shared_ptr<_ProxyClass<_AttributeExtensions...>> buildProxy(const std::string &_domain,
                                                                     const std::string &_instance,
                                                                     std::shared_ptr<MainLoopContext>)
    {
        (void)_domain;
        (void)_instance;
        if (available)
        {
            if (proxyDelegate)
            {
                std::shared_ptr<_ProxyClass<_AttributeExtensions...>> proxy =
                    std::make_shared<_ProxyClass<_AttributeExtensions...>>(proxyDelegate);
                return proxy;
            }
            else
            {
                std::shared_ptr<_ProxyClass<_AttributeExtensions...>> proxy =
                    std::make_shared<_ProxyClass<_AttributeExtensions...>>(nullptr);
                return proxy;
            }
        }
        std::cout << "Non-available nullptr" << std::endl;
        return nullptr;
    }

    template <typename T>
    bool registerService(const std::string &, const std::string &, std::shared_ptr<T>, std::shared_ptr<MainLoopContext>)
    {
        return true;
    }

    template <typename T>
    bool registerService(const std::string &,
                         const std::string &,
                         std::shared_ptr<T>,
                         const CommonAPI::ConnectionId_t &)
    {
        return true;
    }

    bool unregisterService(const std::string &_domain, const std::string &_interface, const std::string &_instance)
    {
        (void)_domain;
        (void)_interface;
        (void)_instance;
        return true;
    }

    bool isAvailable()
    {
        return available;
    }
};
}

#endif /* MOCK_COMMONAPI_RUNTIME_HPP_ */
