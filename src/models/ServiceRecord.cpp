#include "ServiceRecord.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>


ServiceRecord::ServiceRecord(QObject* parent) : QObject{ parent }
{
  connect(this, &ServiceRecord::repeatAfterDistanceChanged, this, &ServiceRecord::isRepeatDistanceValidChanged);
  connect(this, &ServiceRecord::hasRepeatAfterDistanceChanged, this, &ServiceRecord::isRepeatDistanceValidChanged);
  connect(this, &ServiceRecord::eventTypeChanged, this, &ServiceRecord::isRepeatDistanceValidChanged);
  connect(this, &ServiceRecord::isRepeatDistanceValidChanged, this, &ServiceRecord::isValidChanged);

  connect(this, &ServiceRecord::repeatAfterMonthsChanged, this, &ServiceRecord::isRepeatMonthsValidChanged);
  connect(this, &ServiceRecord::hasRepeatAfterMonthsChanged, this, &ServiceRecord::isRepeatMonthsValidChanged);
  connect(this, &ServiceRecord::eventTypeChanged, this, &ServiceRecord::isRepeatMonthsValidChanged);
  connect(this, &ServiceRecord::isRepeatMonthsValidChanged, this, &ServiceRecord::isValidChanged);

  connect(this, &ServiceRecord::nameChanged, this, &ServiceRecord::isNameValidChanged);
  connect(this, &ServiceRecord::isNameValidChanged, this, &ServiceRecord::isValidChanged);

  connect(this, &ServiceRecord::notesChanged, this, &ServiceRecord::isValidChanged);
  connect(this, &ServiceRecord::priceChanged, this, &ServiceRecord::isValidChanged);
  connect(this, &ServiceRecord::mileageChanged, this, &ServiceRecord::isValidChanged);
  connect(this, &ServiceRecord::serviceDateChanged, this, &ServiceRecord::isValidChanged);
}

QString ServiceRecord::toJSON() const
{
  QMetaEnum metaEnum = QMetaEnum::fromType<ServiceRecord::EventType>();

  QJsonObject json{ { "eventType", QString::fromUtf8(metaEnum.valueToKey(m_eventType)) },
                    { "name", m_name },
                    { "notes", m_notes },
                    { "price", m_price },
                    { "mileage", m_mileage },
                    { "serviceDate", m_serviceDate.toString(Qt::ISODate) },
                    { "repeatAfterDistance", m_repeatAfterDistance },
                    { "hasRepeatAfterDistance", m_hasRepeatAfterDistance },
                    { "repeatAfterMonths", m_repeatAfterMonths },
                    { "hasRepeatAfterMonths", m_hasRepeatAfterMonths } };
  return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

ServiceRecord* ServiceRecord::fromJSON(const QString& jsonString, QObject* parent)
{
  ServiceRecord* serviceRecord = new ServiceRecord(parent);
  serviceRecord->fromJSONString(jsonString);
  return serviceRecord;
}

QString ServiceRecord::name() const
{
  return m_name;
}

void ServiceRecord::setName(const QString& newName)
{
  if (m_name == newName)
    return;
  m_name = newName;
  emit nameChanged();
}

QString ServiceRecord::notes() const
{
  return m_notes;
}

void ServiceRecord::setNotes(const QString& newNotes)
{
  if (m_notes == newNotes)
    return;
  m_notes = newNotes;
  emit notesChanged();
}

int ServiceRecord::price() const
{
  return m_price;
}

void ServiceRecord::setPrice(int newPrice)
{
  if (m_price == newPrice)
    return;
  m_price = newPrice;
  emit priceChanged();
}

int ServiceRecord::mileage() const
{
  return m_mileage;
}

void ServiceRecord::setMileage(int newMileage)
{
  if (m_mileage == newMileage)
    return;
  m_mileage = newMileage;
  emit mileageChanged();
}

QDate ServiceRecord::serviceDate() const
{
  return m_serviceDate;
}

void ServiceRecord::setServiceDate(const QDate& newServiceDate)
{
  if (m_serviceDate == newServiceDate)
    return;
  m_serviceDate = newServiceDate;
  emit serviceDateChanged();
}

int ServiceRecord::repeatAfterDistance() const
{
  if (m_eventType == EventType::Maintenance)
  {
    return m_repeatAfterDistance;
  }
  return 0;
}

void ServiceRecord::setRepeatAfterDistance(int newRepeatAfterDistance)
{
  if (m_repeatAfterDistance == newRepeatAfterDistance)
    return;

  m_repeatAfterDistance = newRepeatAfterDistance;
  emit repeatAfterDistanceChanged();
}

bool ServiceRecord::hasRepeatAfterDistance() const
{
  if (m_eventType == EventType::Maintenance)
  {
    return m_hasRepeatAfterDistance;
  }
  return false;
}

void ServiceRecord::setHasRepeatAfterDistance(bool newHasRepeatAfterDistance)
{
  if (m_hasRepeatAfterDistance == newHasRepeatAfterDistance)
    return;
  m_hasRepeatAfterDistance = newHasRepeatAfterDistance;
  emit hasRepeatAfterDistanceChanged();
}

int ServiceRecord::repeatAfterMonths() const
{
  if (m_eventType == EventType::Maintenance)
  {
    return m_repeatAfterMonths;
  }
  return 0;
}

void ServiceRecord::setRepeatAfterMonths(int newRepeatAfterMonths)
{
  if (m_repeatAfterMonths == newRepeatAfterMonths)
    return;
  m_repeatAfterMonths = newRepeatAfterMonths;
  emit repeatAfterMonthsChanged();
}

bool ServiceRecord::hasRepeatAfterMonths() const
{
  if (m_eventType == EventType::Maintenance)
  {
    return m_hasRepeatAfterMonths;
  }
  return false;
}

void ServiceRecord::setHasRepeatAfterMonths(bool newHasRepeatAfterMonths)
{
  if (m_hasRepeatAfterMonths == newHasRepeatAfterMonths)
    return;
  m_hasRepeatAfterMonths = newHasRepeatAfterMonths;
  emit hasRepeatAfterMonthsChanged();
}

void ServiceRecord::fromJSONString(const QString& jsonString)
{
  QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
  if (!doc.isNull() && doc.isObject())
  {
    QJsonObject json = doc.object();

    QString eventTypeStr = json["eventType"].toString();

    if (!eventTypeStr.isEmpty())
    {
      QMetaEnum metaEnum = QMetaEnum::fromType<ServiceRecord::EventType>();
      bool      ok       = false;
      int       value    = metaEnum.keyToValue(eventTypeStr.toUtf8().constData(), &ok);
      if (ok)
      {
        setEventType(static_cast<EventType>(value));
      }
      else
      {
        setEventType(Maintenance);
        qWarning() << "Unknown event type: " << eventTypeStr;
      }
    }

    setName(json["name"].toString());
    setNotes(json["notes"].toString());
    setPrice(json["price"].toInt());
    setMileage(json["mileage"].toInt());
    setServiceDate(QDate::fromString(json["serviceDate"].toString()));

    setRepeatAfterDistance(json["repeatAfterDistance"].toInt());
    setRepeatAfterMonths(json["repeatAfterMonths"].toInt());
    setHasRepeatAfterDistance(json["hasRepeatAfterDistance"].toBool());
    setHasRepeatAfterMonths(json["hasRepeatAfterMonths"].toBool());
  }
}

bool ServiceRecord::isNameValid() const
{
  return !m_name.isEmpty();
}

bool ServiceRecord::isValid() const
{
  return isRepeatDistanceValid() && isRepeatMonthsValid() && isNameValid();
}

ServiceRecord::EventType ServiceRecord::eventType() const
{
  return m_eventType;
}

void ServiceRecord::setEventType(const EventType& newEventType)
{
  if (m_eventType == newEventType)
    return;
  m_eventType = newEventType;
  emit eventTypeChanged();
}

bool ServiceRecord::isRepeatDistanceValid() const
{
  if (m_eventType == EventType::Maintenance)
  {
    if (m_hasRepeatAfterDistance)
    {
      return m_repeatAfterDistance > 0;
    }
  }
  return true;
}

bool ServiceRecord::isRepeatMonthsValid() const
{
  if (m_eventType == EventType::Maintenance)
  {
    if (m_hasRepeatAfterMonths)
    {
      return m_repeatAfterMonths > 0;
    }
  }
  return true;
}
