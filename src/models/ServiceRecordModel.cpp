#include "ServiceRecordModel.h"
#include "ServiceRecord.h"
#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStandardPaths>
#include <ServiceRecordBuilder.h>

namespace {
constexpr QStringView tableName = u"service_records";
}

ServiceRecordModel::ServiceRecordModel(const QSqlDatabase& db, QObject* parent)
   : QAbstractListModel{parent}
{
    if (db.isValid())
    {
        createTableIfNotExists(db);
        m_model = new QSqlTableModel(this, db);
    }
    else
    {
        QSqlDatabase myDb = openDatabase();
        createTableIfNotExists(myDb);
        m_model = new QSqlTableModel(this, myDb);
    }

    m_model->setTable(tableName.toString());
    m_model->setEditStrategy(QSqlTableModel::OnRowChange);
    if (!m_model->select())
    {
        qWarning() << "Failed to select data" << m_model->lastError().text();
    }
}

int ServiceRecordModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid() || !m_model)
    {
        return 0;
    }
    return m_model->rowCount();
}

QVariant ServiceRecordModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || !m_model)
    {
        return {};
    }

    const QSqlRecord record = m_model->record(index.row());

    switch (role)
    {
        case RecordIdRole:
            return record.value("record_id");
        case EventTypeRole:
            return record.value("event_type");
        case NameRole:
            return record.value("name");
        case NotesRole:
            return record.value("notes");
        case PriceRole:
            return record.value("price");
        case MileageRole:
            return record.value("mileage");
        case ServiceDateRole:
            return record.value("service_date").toString();
        case ParentRecordIdRole:
            return record.value("parent_record_id").toInt();
        case RepeatAfterDistanceRole:
            return record.value("repeat_after_distance");
        case HasRepeatAfterDistanceRole:
            return record.value("has_repeat_after_distance").toBool();
        case RepeatAfterMonthsRole:
            return record.value("repeat_after_months");
        case HasRepeatAfterMonthsRole:
            return record.value("has_repeat_after_months").toBool();
        default:
            return {};
    }
}

QHash<int, QByteArray> ServiceRecordModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[RecordIdRole] = "recordId";
    roles[EventTypeRole] = "eventType";
    roles[NameRole] = "name";
    roles[NotesRole] = "notes";
    roles[PriceRole] = "price";
    roles[MileageRole] = "mileage";
    roles[ServiceDateRole] = "serviceDate";
    roles[ParentRecordIdRole] = "parentRecordId";
    roles[RepeatAfterDistanceRole] = "repeatAfterDistance";
    roles[HasRepeatAfterDistanceRole] = "hasRepeatAfterDistance";
    roles[RepeatAfterMonthsRole] = "repeatAfterMonths";
    roles[HasRepeatAfterMonthsRole] = "hasRepeatAfterMonths";
    return roles;
}

int ServiceRecordModel::append(ServiceRecord* record)
{
    if (!record || !m_model)
    {
        return -1;
    }

    QSqlRecord sqlRecord = sqlRecordFromServiceRecord(record);

    const int newRow = m_model->rowCount();

    if (!m_model->insertRecord(newRow, sqlRecord))
    {
        qWarning() << "Failed to append record:" << m_model->lastError().text();
        return -1;
    }

    if (!m_model->submitAll())
    {
        qWarning() << "Failed to submit append:" << m_model->lastError().text();
        m_model->revertAll();
        m_model->select();
        return -1;
    }

    qDebug() << "Record appended successfully";

    beginInsertRows(QModelIndex(), newRow, newRow);
    m_model->select();
    endInsertRows();
    return m_model->record(newRow).value("record_id").toInt();
}

bool ServiceRecordModel::clear()
{
    if (m_model->rowCount() == 0)
    {
        return true;
    }

    QSqlQuery query(m_model->database());
    if (!query.exec(QString("DELETE FROM %1").arg(tableName)))
    {
        qWarning() << "DELETE failed:" << query.lastError().text();
        return false;
    }

    beginResetModel();
    m_model->select();
    endResetModel();

    return true;
}

void ServiceRecordModel::removeById(int recordId)
{
    if (auto index = indexById(recordId); index.has_value())
    {
        removeByIndex(index.value());
    }
}

ServiceRecord* ServiceRecordModel::getById(int recordId, QObject* parent) const
{
    if (auto index = indexById(recordId); index.has_value())
    {
        return getByIndex(index.value(), parent);
    }
    return nullptr;
}

int ServiceRecordModel::count() const
{
    return rowCount();
}

bool ServiceRecordModel::updateRecordById(int recordId, ServiceRecord* sr)
{
    const auto& toString = ServiceRecordBuilder::eventType2Str;

    auto index = indexById(recordId);
    if (!index)
    {
        qWarning() << "Cannot found record with id" << recordId;
        return false;
    }

    const auto row = index.value();

    auto record = m_model->record(row);

    record.setValue("event_type", toString(sr->eventType()));
    record.setValue("name", sr->name());
    record.setValue("notes", sr->notes());
    record.setValue("price", sr->price());
    record.setValue("mileage", sr->mileage());
    record.setValue("service_date", sr->serviceDate().toString(Qt::ISODate));
    record.setValue("parent_record_id", sr->parentRecordId());
    record.setValue("repeat_after_distance", sr->repeatAfterDistance());
    record.setValue("has_repeat_after_distance", sr->hasRepeatAfterDistance());
    record.setValue("repeat_after_months", sr->repeatAfterMonths());
    record.setValue("has_repeat_after_months", sr->hasRepeatAfterMonths());

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

    emit dataChanged(this->index(row), this->index(row));
    return true;
}

ServiceRecord* ServiceRecordModel::getByIndex(int index, QObject* parent) const
{
    if (!m_model || index < 0 || index >= m_model->rowCount())
    {
        return nullptr;
    }
    QSqlRecord record = m_model->record(index);
    return serviceRecordFromSqlRecord(record, parent);
}

std::optional<int> ServiceRecordModel::indexById(int recordId) const
{
    if (!m_model)
    {
        return std::nullopt;
    }

    for (int row = 0; row < m_model->rowCount(); ++row)
    {
        QSqlRecord record = m_model->record(row);
        const int currentId = record.value("record_id").toInt();
        if (currentId == recordId)
        {
            return row;
        }
    }
    return std::nullopt;
}

void ServiceRecordModel::removeByIndex(int index)
{
    const int rowCount = m_model->rowCount();

    if (index < 0 || index >= m_model->rowCount())
    {
        return;
    }

    if (!m_model->removeRow(index))
    {
        qWarning() << "Remove row failed:" << m_model->lastError().text();
        return;
    }
    else
    {
        qDebug() << "Removed by index successfully";
    }

    beginRemoveRows(QModelIndex(), index, index);
    endRemoveRows();
}

QSqlDatabase ServiceRecordModel::openDatabase()
{
    const QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
                           + "/bibika_service.sqlite";
    const QString connectionName = "service_records_connection";

    QSqlDatabase db;

    if (QSqlDatabase::contains(connectionName))
    {
        db = QSqlDatabase::database(connectionName);
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        if (!db.isValid())
        {
            qWarning() << "Cannot add database" << db.lastError().text();
        }
        db.setDatabaseName(dbPath);
    }

    if (!db.open())
    {
        qWarning() << "Cannot open database" << db.lastError().text();
    }

    return db;
}

void ServiceRecordModel::createTableIfNotExists(const QSqlDatabase& db)
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

QSqlRecord ServiceRecordModel::sqlRecordFromServiceRecord(ServiceRecord* sr) const
{
    const auto& toString = ServiceRecordBuilder::eventType2Str;

    QSqlRecord record = m_model->record();

    record.setValue("event_type", toString(sr->eventType()));
    record.setValue("name", sr->name());
    record.setValue("notes", sr->notes());
    record.setValue("price", sr->price());
    record.setValue("mileage", sr->mileage());
    record.setValue("service_date", sr->serviceDate().toString(Qt::ISODate));
    record.setValue("repeat_after_distance", sr->repeatAfterDistance());
    record.setValue("has_repeat_after_distance", sr->hasRepeatAfterDistance() ? 1 : 0);
    record.setValue("repeat_after_months", sr->repeatAfterMonths());
    record.setValue("has_repeat_after_months", sr->hasRepeatAfterMonths() ? 1 : 0);

    return record;
}

ServiceRecord* ServiceRecordModel::serviceRecordFromSqlRecord(const QSqlRecord& record, QObject* parent) const
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
