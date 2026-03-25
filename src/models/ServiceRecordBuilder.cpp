#include "ServiceRecordBuilder.h"
#include "ServiceRecord.h"

#include <QJsonDocument>
#include <QJsonObject>

ServiceRecordBuilder::ServiceRecordBuilder(QObject* parent)
   : QObject{parent}
{}

ServiceRecord* ServiceRecordBuilder::createEmpty(QObject* parent)
{
    return new ServiceRecord(parent);
}

ServiceRecord* ServiceRecordBuilder::fromJSON(const QString& jsonString, QObject* parent)
{
    if (jsonString.isEmpty())
    {
        return new ServiceRecord(parent);
    }

    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (doc.isNull() || !doc.isObject())
    {
        qWarning() << "Invalid JSON:" << jsonString;
        return nullptr;
    }

    QJsonObject json = doc.object();

    ServiceRecord* sr = new ServiceRecord(parent);

    auto setField = [&](auto key, auto converter, auto setter) {
        if (json.contains(key))
        {
            const auto value = converter(json[key]);
            (sr->*setter)(value);
        }
    };

    setField(
       "eventType",
       [](const auto& v) {
           return str2EventType(v.toString());
       },
       &ServiceRecord::setEventType);

    setField(
       "name",
       [](const auto& v) {
           return v.toString();
       },
       &ServiceRecord::setName);

    setField(
       "notes",
       [](const auto& v) {
           return v.toString();
       },
       &ServiceRecord::setNotes);

    setField(
       "price",
       [](const auto& v) {
           return v.toInt();
       },
       &ServiceRecord::setPrice);

    setField(
       "mileage",
       [](const auto& v) {
           return v.toInt();
       },
       &ServiceRecord::setMileage);

    setField(
       "serviceDate",
       [](const auto& v) {
           return QDate::fromString(v.toString(), Qt::ISODate);
       },
       &ServiceRecord::setServiceDate);

    setField(
       "repeatAfterDistance",
       [](const auto& v) {
           return v.toInt();
       },
       &ServiceRecord::setRepeatAfterDistance);

    setField(
       "repeatAfterMonths",
       [](const auto& v) {
           return v.toInt();
       },
       &ServiceRecord::setRepeatAfterMonths);

    setField(
       "hasRepeatAfterDistance",
       [](const auto& v) {
           return v.toBool();
       },
       &ServiceRecord::setHasRepeatAfterDistance);

    setField(
       "hasRepeatAfterMonths",
       [](const auto& v) {
           return v.toBool();
       },
       &ServiceRecord::setHasRepeatAfterMonths);

    return sr;
}

QString ServiceRecordBuilder::toJSON(ServiceRecord* sr)
{
    if (!sr)
    {
        return "{}";
    }

    QJsonObject json{{"eventType", eventType2Str(sr->eventType())},
                     {"name", sr->name()},
                     {"notes", sr->notes()},
                     {"price", sr->price()},
                     {"mileage", sr->mileage()},
                     {"serviceDate", sr->serviceDate().toString(Qt::ISODate)},
                     {"repeatAfterDistance", sr->repeatAfterDistance()},
                     {"hasRepeatAfterDistance", sr->hasRepeatAfterDistance()},
                     {"repeatAfterMonths", sr->repeatAfterMonths()},
                     {"hasRepeatAfterMonths", sr->hasRepeatAfterMonths()}};

    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString ServiceRecordBuilder::eventType2Str(ServiceRecord::EventType eventType)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<ServiceRecord::EventType>();
    return QString::fromUtf8(metaEnum.valueToKey(static_cast<int>(eventType)));
}

ServiceRecord::EventType ServiceRecordBuilder::str2EventType(const QString& eventTypeStr)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<ServiceRecord::EventType>();
    bool ok = false;
    const int value = metaEnum.keyToValue(eventTypeStr.toUtf8(), &ok);
    if (!ok)
    {
        qWarning() << "Cannot convert" << eventTypeStr << "to EventType";
        return ServiceRecord::Invalid;
    }
    return static_cast<ServiceRecord::EventType>(value);
}
