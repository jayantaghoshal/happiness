
#ifndef JSONSERIALIZERS_H
#define JSONSERIALIZERS_H

#include "gen_jsonencdec.h"

// ==================================================================
// The following two json serialize/deserialize shall be in another file
template<typename S>
QByteArray serialize(const DataElemValue<S>& dev) {
    using value_type = typename S::data_elem_type;

    QJsonObject jsonObject;
    if (dev.isOk()) jsonObject["value"] = autosar::toJson<value_type>(dev.value());
    jsonObject["timestamp"] = dev.timestamp();
    jsonObject["state"] = (int)dev.state();
    jsonObject["type"] = S::TypeName();
    if (dev.isError()) jsonObject["errorCode"] = dev.errorCode();

    QJsonDocument d(jsonObject);
    return d.toJson();
}

template<typename S>
DataElemValue<S> deserialize(const QByteArray& ba) {
    using value_type = typename S::data_elem_type;

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(ba,&error);
    if (jsonDoc.isNull() || !jsonDoc.isObject() || error.error!=QJsonParseError::NoError)
    {
        // TODO exception
        return DataElemValue<S>::ERROR(0);
    }

    QJsonObject object = jsonDoc.object();

    QJsonValue typeJson = object.value("type");
    if (typeJson.isUndefined()) {
        // TODO exception ??
        return DataElemValue<S>::ERROR(-1, object.value("timestamp").toDouble(-1)); // mandatory type missing
    }

    if (typeJson.toString().toStdString()!=S::TypeName()) {
        qDebug() << "Is trying to deserialize " << typeJson.toString().toStdString().c_str() << "into " << S::TypeName();
        // TODO exception ??
        return DataElemValue<S>::ERROR(-1, object.value("timestamp").toDouble(-1)); // mandatory state missing
    }

    QJsonValue stateJson = object.value("state");
    if (stateJson.isUndefined()) {
        // TODO exception ??
        return DataElemValue<S>::ERROR(-1, object.value("timestamp").toDouble(-1)); // mandatory state missing
    }

    DataElemValueBase::State state = (DataElemValueBase::State)stateJson.toInt();
    switch(state) {
    case DataElemValueBase::State::OK:
    {
        const value_type value = autosar::fromJson<value_type>(object.value("value"));
        return DataElemValue<S>::OK(value, object.value("timestamp").toDouble(-1));
        break;
    }

    case DataElemValueBase::State::ERROR:
        return DataElemValue<S>::ERROR(object.value("errorCode").toInt(0), object.value("timestamp").toDouble(-1));
        break;

    default:
        return DataElemValue<S>::ERROR(-2, object.value("timestamp").toDouble(-1)); // unknown state
    }
}

#endif // JSONSERIALIZERS_H
