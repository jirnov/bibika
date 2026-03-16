#include "ServiceRecordBuilder.h"
#include "ServiceRecord.h"


ServiceRecordBuilder::ServiceRecordBuilder(QObject* parent) : QObject{ parent }
{
}

ServiceRecord* ServiceRecordBuilder::create(const QVariantMap& data, QObject* parent)
{
  auto* record = new ServiceRecord(parent);

  auto setIfContains = [&](const QString& key, auto setter) {
    if (data.contains(key))
    {
      setter(data[key]);
    }
  };

  setIfContains("name", [&](const auto& v) { record->setName(v.toString()); });
  setIfContains("notes", [&](const auto& v) { record->setNotes(v.toString()); });

  setIfContains("price", [&](const QVariant& v) {
    bool ok;
    int  val = v.toInt(&ok);
    if (ok)
      record->setPrice(val);
  });

  setIfContains("mileage", [&](const QVariant& v) {
    bool ok;
    int  val = v.toInt(&ok);
    if (ok)
      record->setMileage(val);
  });

  setIfContains("serviceDate", [&](const QVariant& v) {
    if (v.canConvert<QDate>())
    {
      record->setServiceDate(v.toDate());
    }
    else
    {
      QDate date = QDate::fromString(v.toString(), Qt::ISODate);
      if (date.isValid())
        record->setServiceDate(date);
    }
  });

  setIfContains("eventType", [&](const QVariant& v) {
    bool ok;
    int  val = v.toInt(&ok);
    if (ok && val >= ServiceRecord::Repair && val <= ServiceRecord::Service)
    {
      record->setEventType(static_cast<ServiceRecord::EventType>(val));
    }
  });

  setIfContains("repeatAfterDistance", [&](const QVariant& v) {
    bool ok;
    int  val = v.toInt(&ok);
    if (ok)
      record->setRepeatAfterDistance(val);
  });

  setIfContains("hasRepeatAfterDistance", [&](const QVariant& v) { record->setHasRepeatAfterDistance(v.toBool()); });

  setIfContains("repeatAfterMonths", [&](const QVariant& v) {
    bool ok;
    int  val = v.toInt(&ok);
    if (ok)
      record->setRepeatAfterMonths(val);
  });

  setIfContains("hasRepeatAfterMonths", [&](const QVariant& v) { record->setHasRepeatAfterMonths(v.toBool()); });

  return record;
}

ServiceRecord* ServiceRecordBuilder::createEmpty(QObject* parent)
{
  return new ServiceRecord(parent);
}

ServiceRecord* ServiceRecordBuilder::fromJSON(const QString& jsonString, QObject* parent)
{
  return ServiceRecord::fromJSON(jsonString, parent);
}
