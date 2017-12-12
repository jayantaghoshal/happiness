/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <algorithm>
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

/*
Funny structure to make sure destruction order of NotifiableProperty vs SubscriptionHandle
doesn't matter.


(The application Subscription handle could theoretically be a shared_ptr right away
instead of being a unique_ptr to a shared_ptr but i thought the current structure
shows the intent better, the application should be fully responsible for keeping the
handle alive. What we really want is something like a "weak_ptr" to a unique_ptr but no such thing exist)

Once the shared_ptr inside Subscription is destroyed, the subscription is considered canceled and the weak_ptr
will fail to get a lock in the notifyListeners()-method. Only then will it be removed from
the listeners-list. If the the weak_ptr is beeing locked while the SubscriptionData is being destroyed, it will
hold the SubscriptionData alive during notification-period but then the
subscriptionDestroyed-member will protect the callback from triggering.

This could lead to an ever growing list of listeners if listeners are added and removed and
notify is never called but this risk is very small and properties should generally have very few listeners anyway.

|------------------------
|  NotifiableProperty   |
|   - listeners[]       |
|-------- | -------------
          |
          |weak
          V
|---------------------------   shared |------------------------
|  SubscriptionData        |<---------| Subscription          |
|  - callback              |          |                       |
|  - subscriptionDestroyed |          |                       |
|---------------------------          |------------------------
                                                 ^
                                                 | unique
                                      |------------------------
                                      | Application           |
                                      |                       |
                                      |------------------------

*/

template <typename T>
class NotifiableProperty;

template <typename T>
class SubscriptionData;

class SubscriptionBase {
  public:
    virtual ~SubscriptionBase() = default;
};

using SubscriptionHandle = std::unique_ptr<SubscriptionBase>;

template <typename T>
class Subscription : public SubscriptionBase {
  public:
    Subscription(std::shared_ptr<SubscriptionData<T>> subData) : subData{subData} {}
    virtual ~Subscription() { subData->setSubscriptionDestroyed(); }

  private:
    const std::shared_ptr<SubscriptionData<T>> subData;
};

template <typename T>
class SubscriptionData {
  public:
    SubscriptionData(std::function<void(T)> callback) : callback(callback) {}
    void fireCallbackIfSubscriptionStillAlive(const T value) const {
        if (!subscriptionDestroyed) callback(value);
    }
    void setSubscriptionDestroyed() { subscriptionDestroyed = true; }

  private:
    const std::function<void(T)> callback;
    // Used to prevent race conditions between weak_ptr-lock and destruction of Subscription
    std::atomic<bool> subscriptionDestroyed{false};
};

template <>
class SubscriptionData<void> {
  public:
    SubscriptionData(std::function<void()> callback) : callback(callback) {}
    void fireCallbackIfSubscriptionStillAlive() const {
        if (!subscriptionDestroyed) callback();
    }
    void setSubscriptionDestroyed() { subscriptionDestroyed = true; }

  private:
    const std::function<void()> callback;
    // Used to prevent race conditions between weak_ptr-lock and destruction of Subscription
    std::atomic<bool> subscriptionDestroyed{false};
};

// Pure virtual class / interface
template <typename T>
class ReadOnlyNotifiableProperty {
  public:
    virtual ~ReadOnlyNotifiableProperty() = default;

    // Unsubscribing is done by destroying the SubscriptionHandle
    virtual SubscriptionHandle subscribeAndCall(std::function<void(T value)>&& listener) = 0;
    virtual SubscriptionHandle subscribe(std::function<void(T value)>&& listener) = 0;

    virtual T get() const = 0;
};

template <typename T>
class NotifiableProperty : virtual public ReadOnlyNotifiableProperty<T> {
  public:
    NotifiableProperty() : value_{T{}} {}

    NotifiableProperty(T value) : value_{value} {}

    virtual ~NotifiableProperty() = default;

    virtual SubscriptionHandle subscribeAndCall(std::function<void(T value)>&& listener) override {
        T valueReadInMutex;
        {
            std::unique_lock<std::mutex> lock{valueMutex_};
            valueReadInMutex = value_;
        }
        listener(valueReadInMutex);
        return subscribe(std::move(listener));
    }

    virtual SubscriptionHandle subscribe(std::function<void(T value)>&& listener) override {
        auto subData = std::make_shared<SubscriptionData<T>>(listener);
        {
            std::unique_lock<std::mutex> lock{listenersMutex_};
            listeners_.emplace_back(subData);
        }
        return std::make_unique<Subscription<T>>(subData);
    }

    virtual T get() const override {
        std::unique_lock<std::mutex> lock{valueMutex_};
        return value_;
    }

    template <typename... Args>
    void set(Args&&... values) {
        set({std::forward<Args>(values)...});
    }

    void set(T value) {
        {
            std::unique_lock<std::mutex> lock{valueMutex_};
            if (value_ == value) {
                return;
            }
            value_ = value;
        }
        notifyListeners();
    }

    void notifyListeners() {
        std::vector<std::shared_ptr<SubscriptionData<T>>> toNotify;
        {
            std::unique_lock<std::mutex> lock{listenersMutex_};

            auto it = std::begin(listeners_);
            while (it != std::end(listeners_)) {
                std::weak_ptr<SubscriptionData<T>> listener = *it;
                auto l = listener.lock();
                if (l != nullptr) {
                    toNotify.push_back(l);
                    ++it;
                } else {
                    it = listeners_.erase(it);
                }
            }
        }

        for (const auto& l : toNotify) {
            l->fireCallbackIfSubscriptionStillAlive(value_);
        }
    }

  private:
    T value_;
    std::vector<std::weak_ptr<SubscriptionData<T>>> listeners_;
    std::mutex listenersMutex_;
    mutable std::mutex valueMutex_;
};

// The following is a specialization for notifiable properties with no payloads
// TODO: Move to another file and rename to NotifiableEvent or something similar

template <>
class ReadOnlyNotifiableProperty<void> {
  public:
    virtual ~ReadOnlyNotifiableProperty() = default;
    virtual SubscriptionHandle subscribe(std::function<void()>&& listener) = 0;
};

template <>
class NotifiableProperty<void> : public ReadOnlyNotifiableProperty<void> {
  public:
    virtual ~NotifiableProperty() = default;

    virtual SubscriptionHandle subscribe(std::function<void()>&& listener) override {
        auto subData = std::make_shared<SubscriptionData<void>>(listener);
        {
            std::unique_lock<std::mutex> lock{listenersMutex_};
            listeners_.emplace_back(subData);
        }
        return std::make_unique<Subscription<void>>(subData);
    }

    void set() { notifyListeners(); }

    void notifyListeners() {
        std::vector<std::shared_ptr<SubscriptionData<void>>> toNotify;
        {
            std::unique_lock<std::mutex> lock{listenersMutex_};

            auto it = std::begin(listeners_);
            while (it != std::end(listeners_)) {
                std::weak_ptr<SubscriptionData<void>> listener = *it;
                auto l = listener.lock();
                if (l != nullptr) {
                    toNotify.push_back(l);
                    ++it;
                } else {
                    it = listeners_.erase(it);
                }
            }
        }

        for (const auto& l : toNotify) {
            l->fireCallbackIfSubscriptionStillAlive();
        }
    }

  private:
    std::vector<std::weak_ptr<SubscriptionData<void>>> listeners_;
    std::mutex listenersMutex_;
};
