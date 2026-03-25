#include "ServiceRecord.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>

ServiceRecord::ServiceRecord(QObject* parent)
   : QObject{parent}
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

QString ServiceRecord::name() const
{
    return m_name;
}

void ServiceRecord::setName(const QString& newName)
{
    if (m_name == newName)
    {
        return;
    }
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
    {
        return;
    }
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
    {
        return;
    }
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
    {
        return;
    }
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
    {
        return;
    }
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
    {
        return;
    }

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
    {
        return;
    }
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
    {
        return;
    }
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
    {
        return;
    }
    m_hasRepeatAfterMonths = newHasRepeatAfterMonths;
    emit hasRepeatAfterMonthsChanged();
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
    {
        return;
    }
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

int ServiceRecord::parentRecordId() const
{
    return m_parentRecordId;
}

void ServiceRecord::setParentRecordId(int newParentRecordId)
{
    m_parentRecordId = newParentRecordId;
}
