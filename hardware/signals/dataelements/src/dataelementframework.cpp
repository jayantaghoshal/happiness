/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "dataelementframework.h"

#undef LOG_TAG
#define LOG_TAG "DataElementFramework"
#include <cutils/log.h>

#include <sys/types.h>


DataElementFramework& DataElementFramework::instance() {
  static DataElementFramework theInstance;
  return theInstance;
}

DataElementFramework::DataElementFramework() : m_commBus(IDataElementCommBus::create()) {
  m_commBus->setNewDataElementHandler([this](const std::string& name, const std::string& payload) {
    std::lock_guard<std::recursive_mutex> safeLock(mutex_protectDataElementListeners);

    auto iter = m_dataElementListeners.find(name);
    if (iter != m_dataElementListeners.end()) {
      const std::list<INewDataElement*>& subs = iter->second.second;
      DataElemValueBase& devb = *iter->second.first;
      devb.deserialize(payload);
      for (auto ptr : subs) { // update all values
        ptr->newDataElementValue(devb);
      }
      for (auto ptr : subs) { // perform all callbacks
        ptr->performCallback();
      }
    }
  });
}

void DataElementFramework::unsubscribe(const std::string& name, INewDataElement* callback) {
  std::lock_guard<std::recursive_mutex> safeLock(mutex_protectDataElementListeners);

  auto iter = m_dataElementListeners.find(name);
  if (iter != m_dataElementListeners.end()) {
    MapValue& mv = iter->second;
    auto i = std::find(mv.second.begin(), mv.second.end(), callback);
    if (i != mv.second.end()) {
      mv.second.erase(i);
    }
  }
}

void DataElementFramework::_inject(const DataElemValueBase& dataElem) {
  const std::string ba = dataElem.serialize();
  m_commBus->send(dataElem.name(), ba, dataElem.direction());
}

void DataElementFramework::subscribe(std::unique_ptr<DataElemValueBase> dev, INewDataElement* callback) {
  const autosar::Dir dir = dev->direction();

  // See if we already have a subscription for this
  bool haveSub = true;
  const std::string name = dev->name();
  {  // lock-scope
    std::lock_guard<std::recursive_mutex> safeLock(mutex_protectDataElementListeners);

    auto iter = m_dataElementListeners.find(name);
    if (iter == m_dataElementListeners.end()) {
      // no we don't so lets add it
      haveSub = false;
      auto mapvalue = std::make_pair(std::move(dev), std::list<INewDataElement*>());
      m_dataElementListeners[name] = std::move(mapvalue);
      iter = m_dataElementListeners.find(name);
    }
    MapValue& mv = iter->second;
    if (callback != nullptr) mv.second.push_back(callback);
  }

  if (!haveSub) m_commBus->addName(dir, name);
}

const DataElemValueBase* DataElementFramework::get(const std::string& name) const {
  std::lock_guard<std::recursive_mutex> safeLock(mutex_protectDataElementListeners);

  auto iter = m_dataElementListeners.find(name);
  if (iter != m_dataElementListeners.end()) {
    return iter->second.first.get();
  } else {
    return nullptr;
  }
}

#ifdef UNIT_TEST
void DataElementFramework::reset()
{
    // set all existing to ERROR(-1)
    auto iter = m_dataElementListeners.begin();
    while(iter!=m_dataElementListeners.end())
    {
      const std::list<INewDataElement*>& subs = iter->second.second;
      DataElemValueBase& devb = *iter->second.first;
      devb._errorCode = (int)SignalErrorCode::NEVER_RECEIVED;
      devb._state = DataElemValueBase::State::ERROR;
      for (auto ptr : subs) { // update all values
        ptr->newDataElementValue(devb);
      }
      ++iter;
    }
}

#endif
