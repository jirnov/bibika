#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStandardPaths>
#include <ServiceRecord.h>
#include <ServiceRecordBuilder.h>
#include <ServiceRecordRepository.h>

namespace {
constexpr QStringView tableName = u"service_records";
}

ServiceRecordRepository::ServiceRecordRepository(const QSqlDatabase& db, QObject* parent)
   : QObject{parent}
{
    if (!db.isValid())
    {
        qWarning() << "Invalid database";
        return;
    }

    createTableIfNotExists(db);
    m_model = std::make_unique<QSqlTableModel>(nullptr, db);

    m_model->setTable(tableName.toString());
    m_model->setEditStrategy(QSqlTableModel::OnRowChange);
    if (!m_model->select())
    {
        qWarning() << "Failed to select data" << m_model->lastError().text();
    }
}

ServiceRecord* ServiceRecordRepository::find(int id, QObject* parent) const
{
    if (!m_model)
    {
        return {};
    }

    QSqlQuery query(m_model->database());
    query.prepare(QString("SELECT * FROM %1 WHERE record_id = ?").arg(tableName));
    query.addBindValue(id);

    if (query.exec() && query.next())
    {
        return fromSqlRecord(query.record(), parent);
    }
    return {};
}

QVariant ServiceRecordRepository::data(int index, QAnyStringView fieldName) const
{
    if (!m_model)
    {
        return {};
    }

    const auto record = m_model->record(index);
    return record.value(fieldName);
}

std::optional<int> ServiceRecordRepository::append(const ServiceRecord& newRecord)
{
    if (!m_model)
    {
        return {};
    }

    auto sqlRecord = toSqlRecord(newRecord);
    const int newRow = m_model->rowCount();

    if (!m_model->insertRecord(newRow, sqlRecord))
    {
        qWarning() << "Failed to append record:" << m_model->lastError().text();
        return {};
    }

    if (!m_model->submitAll())
    {
        qWarning() << "Failed to submit append:" << m_model->lastError().text();
        return {};
    }

    qDebug() << "Record appended successfully";

    emit beginInsertRow(newRow);
    m_model->select();
    emit endInsertRow();
    return m_model->record(newRow).value("record_id").toInt();
}

bool ServiceRecordRepository::update(int id, const ServiceRecord& sr)
{
    if (!m_model)
    {
        return false;
    }

    const auto& toString = ServiceRecordBuilder::eventType2Str;

    auto index = indexById(id);
    if (!index)
    {
        qWarning() << "Cannot find record with id" << id;
        return false;
    }

    const auto row = index.value();

    auto record = m_model->record(row);

    record.setValue("event_type", toString(sr.eventType()));
    record.setValue("name", sr.name());
    record.setValue("notes", sr.notes());
    record.setValue("price", sr.price());
    record.setValue("mileage", sr.mileage());
    record.setValue("service_date", sr.serviceDate().toString(Qt::ISODate));
    record.setValue("parent_record_id", sr.parentRecordId());
    record.setValue("repeat_after_distance", sr.repeatAfterDistance());
    record.setValue("has_repeat_after_distance", sr.hasRepeatAfterDistance());
    record.setValue("repeat_after_months", sr.repeatAfterMonths());
    record.setValue("has_repeat_after_months", sr.hasRepeatAfterMonths());

    if (!m_model->setRecord(row, record))
    {
        qWarning() << "setRecord failed:" << m_model->lastError().text();
        return false;
    }

    if (!m_model->submit())
    {
        qWarning() << "Submit failed:" << m_model->lastError().text();
        return false;
    }

    qDebug() << "Record updated successfully";

    emit dataChanged(row);
    return true;
}

bool ServiceRecordRepository::remove(int id)
{
    if (!m_model)
    {
        return false;
    }

    auto index = indexById(id);
    if (!index.has_value())
    {
        qWarning() << "Cannot find record with id" << id;
        return false;
    }

    const auto row = index.value();
    if (!m_model->removeRow(row))
    {
        qWarning() << "Remove row failed:" << m_model->lastError().text();
        return false;
    }

    qDebug() << "Record removed successfully";
    emit beginRemoveRow(row);
    m_model->select();
    emit endRemoveRow();
    return true;
}

int ServiceRecordRepository::count() const
{
    if (m_model)
    {
        return m_model->rowCount();
    }
    return 0;
}

bool ServiceRecordRepository::clear()
{
    if (!m_model)
    {
        return false;
    }

    if (count() == 0)
    {
        return true;
    }

    QSqlQuery query(m_model->database());
    if (!query.exec(QString("DELETE FROM %1").arg(tableName)))
    {
        qWarning() << "DELETE failed:" << query.lastError().text();
        return false;
    }

    qDebug() << "Clear successfully";
    emit beginResetModel();
    m_model->select();
    emit endResetModel();
    return true;
}

std::optional<int> ServiceRecordRepository::indexById(int id) const
{
    if (!m_model)
    {
        return {};
    }

    m_model->select();

    for (int row = 0; row < count(); ++row)
    {
        QSqlRecord record = m_model->record(row);
        const int currentId = record.value("record_id").toInt();
        if (currentId == id)
        {
            return row;
        }
    }
    return {};
}

QSqlRecord ServiceRecordRepository::toSqlRecord(const ServiceRecord& sr) const
{
    const auto& toString = ServiceRecordBuilder::eventType2Str;

    QSqlRecord record = m_model->record();

    record.setValue("event_type", toString(sr.eventType()));
    record.setValue("name", sr.name());
    record.setValue("notes", sr.notes());
    record.setValue("price", sr.price());
    record.setValue("mileage", sr.mileage());
    record.setValue("parent_record_id", sr.parentRecordId());
    record.setValue("service_date", sr.serviceDate().toString(Qt::ISODate));
    record.setValue("repeat_after_distance", sr.repeatAfterDistance());
    record.setValue("has_repeat_after_distance", sr.hasRepeatAfterDistance() ? 1 : 0);
    record.setValue("repeat_after_months", sr.repeatAfterMonths());
    record.setValue("has_repeat_after_months", sr.hasRepeatAfterMonths() ? 1 : 0);

    return record;
}

ServiceRecord* ServiceRecordRepository::fromSqlRecord(const QSqlRecord& record, QObject* parent) const
{
    const auto& toEventType = ServiceRecordBuilder::str2EventType;

    try
    {
        auto* sr = new ServiceRecord(parent);

        sr->setEventType(toEventType(record.value("event_type").toString()));
        sr->setName(record.value("name").toString());
        sr->setNotes(record.value("notes").toString());
        sr->setPrice(record.value("price").toInt());
        sr->setMileage(record.value("mileage").toInt());
        sr->setParentRecordId(record.value("parent_record_id").toInt());

        auto serviceDate = QDate::fromString(record.value("service_date").toString(), Qt::ISODate);
        sr->setServiceDate(serviceDate);

        sr->setRepeatAfterDistance(record.value("repeat_after_distance").toInt());
        sr->setHasRepeatAfterDistance(record.value("has_repeat_after_distance").toBool());
        sr->setRepeatAfterMonths(record.value("repeat_after_months").toInt());
        sr->setHasRepeatAfterMonths(record.value("has_repeat_after_months").toBool());

        return sr;
    }
    catch (const std::bad_alloc& e)
    {
        qWarning() << "serviceRecordFromSqlRecord faield: bad alloc exception with reason" << e.what();
    }
    return nullptr;
}

void ServiceRecordRepository::createTableIfNotExists(const QSqlDatabase& db)
{
    const QString createTableSql =
       "CREATE TABLE IF NOT EXISTS %1 ("
       "record_id INTEGER PRIMARY KEY AUTOINCREMENT, "
       "event_type TEXT NOT NULL, "
       "name TEXT NOT NULL, "
       "notes TEXT, "
       "price REAL, "
       "mileage INTEGER, "
       "service_date TEXT, "
       "parent_record_id INTEGER DEFAULT 0, "
       "repeat_after_distance INTEGER, "
       "has_repeat_after_distance INTEGER DEFAULT 0, "
       "repeat_after_months INTEGER, "
       "has_repeat_after_months INTEGER DEFAULT 0 "
       ")";

    QSqlQuery query(db);
    if (!query.exec(createTableSql.arg(tableName)))
    {
        qWarning() << "Cannot create table " << query.lastError().text();
    }
}
