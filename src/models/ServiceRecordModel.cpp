#include "ServiceRecordModel.h"
#include "ServiceRecord.h"
#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStandardPaths>

ServiceRecordModel::ServiceRecordModel(QObject* parent) : QAbstractListModel{ parent }
{
  QSqlDatabase db = openDatabase();

  m_model = new QSqlTableModel(this, db);
  m_model->setTable("service_records");
  m_model->setSort(m_model->fieldIndex("mileage"), Qt::AscendingOrder);
  m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
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
  case EventTypeRole: {
    auto type = record.value("event_type");
    qDebug() << "eventtype for index" << index.row() << "=" << type.toString();
    return record.value("event_type");
  }
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
  QHash<int, QByteArray> roles      = QAbstractListModel::roleNames();
  roles[RecordIdRole]               = "recordId";
  roles[EventTypeRole]              = "eventType";
  roles[NameRole]                   = "name";
  roles[NotesRole]                  = "notes";
  roles[PriceRole]                  = "price";
  roles[MileageRole]                = "mileage";
  roles[ServiceDateRole]            = "serviceDate";
  roles[RepeatAfterDistanceRole]    = "repeatAfterDistance";
  roles[HasRepeatAfterDistanceRole] = "hasRepeatAfterDistance";
  roles[RepeatAfterMonthsRole]      = "repeatAfterMonths";
  roles[HasRepeatAfterMonthsRole]   = "hasRepeatAfterMonths";
  return roles;
}

void ServiceRecordModel::append(ServiceRecord* record)
{
  if (!record || !m_model)
  {
    return;
  }

  QSqlRecord sqlRecord = sqlRecordFromServiceRecord(record);

  int newRow = m_model->rowCount();
  beginInsertRows(QModelIndex(), newRow, newRow);
  bool success = m_model->insertRecord(newRow, sqlRecord);

  if (success)
  {
    if (m_model->submitAll())
    {
      qDebug() << "Record appended sucessfully";
      m_model->select();
    }
    else
    {
      qWarning() << "Failed to submit append:" << m_model->lastError().text();
      m_model->revertAll();
      m_model->select();
    }
  }
  else
  {
    qWarning() << "Failed to append record:" << m_model->lastError().text();
  }
  endInsertRows();

  QQmlEngine::setObjectOwnership(record, QQmlEngine::CppOwnership);
  record->deleteLater();
}

void ServiceRecordModel::clear()
{
  beginResetModel();
  bool success = m_model->removeRows(0, m_model->rowCount());

  if (success)
  {
    if (m_model->submitAll())
    {
      qDebug() << "Clear database sucessfullly";
      m_model->select();
    }
    else
    {
      qWarning() << "Failed to submit clear: " << m_model->lastError().text();
    }
  }
  else
  {
    qWarning() << "Failed to clear: " << m_model->lastError().text();
  }

  endResetModel();
}

void ServiceRecordModel::removeById(int recordId)
{
  if (auto index = indexById(recordId); index.has_value())
  {
    removeByIndex(index.value());
  }
}

ServiceRecord* ServiceRecordModel::getById(int recordId) const
{
  if (auto index = indexById(recordId); index.has_value())
  {
    return getByIndex(index.value());
  }
  return nullptr;
}

void ServiceRecordModel::updateRecordById(int recordId, ServiceRecord* updateRecord)
{
  if (auto index = indexById(recordId); index.has_value())
  {
    QMetaEnum metaEnum = QMetaEnum::fromType<ServiceRecord::EventType>();

    qDebug() << "recordId " << recordId;
    qDebug() << "updateRecord event_type =" << QString::fromUtf8(metaEnum.valueToKey(updateRecord->eventType()));

    QSqlRecord sqlRecord = m_model->record(index.value());
    sqlRecord.setValue("event_type", QString::fromUtf8(metaEnum.valueToKey(updateRecord->eventType())));
    sqlRecord.setValue("name", updateRecord->name());
    sqlRecord.setValue("notes", updateRecord->notes());
    sqlRecord.setValue("mileage", updateRecord->mileage());
    sqlRecord.setValue("service_date", updateRecord->serviceDate().toString(Qt::ISODate));
    sqlRecord.setValue("repeat_after_distance", updateRecord->repeatAfterDistance());
    sqlRecord.setValue("has_repeat_after_distance", updateRecord->hasRepeatAfterDistance());
    sqlRecord.setValue("repeat_after_months", updateRecord->repeatAfterMonths());
    sqlRecord.setValue("has_repeat_after_months", updateRecord->hasRepeatAfterMonths());

    if (m_model->setRecord(index.value(), sqlRecord))
    {
      if (m_model->submitAll())
      {
        qDebug() << "Record updated sucessfully";
        m_model->selectRow(index.value());
        emit dataChanged(this->index(index.value()), this->index(index.value()));
      }
      else
      {
        qWarning() << "Submit failed: " << m_model->lastError().text();
        m_model->revertAll();
      }
    }
  }
}

ServiceRecord* ServiceRecordModel::getByIndex(int index) const
{
  QSqlRecord record = m_model->record(index);
  return serviceRecordFromSqlRecord(record);
}

std::optional<int> ServiceRecordModel::indexById(int recordId) const
{
  m_model->select();
  for (int row = 0; row < m_model->rowCount(); ++row)
  {
    QSqlRecord record    = m_model->record(row);
    const int  currentId = record.value("record_id").toInt();
    if (currentId == recordId)
    {
      return row;
    }
  }
  return std::nullopt;
}

void ServiceRecordModel::removeByIndex(int index)
{
  if (index < 0 || index >= m_model->rowCount())
  {
    return;
  }

  beginRemoveRows(QModelIndex(), index, index);
  if (m_model->removeRows(index, 1, QModelIndex()))
  {
    if (m_model->submitAll())
    {
      qDebug() << "Removed by index successfully";
    }
    else
    {
      qWarning() << "Submit remove by index failed: " << m_model->lastError().text();
      m_model->revertAll();
    }
  }
  else
  {
    qWarning() << "Remove by index failed: " << m_model->lastError().text();
  }

  endRemoveRows();
}

QSqlDatabase ServiceRecordModel::openDatabase()
{
  const QString dbPath =
    QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/bibika_service.sqlite";
  const QString connectionName = "service_records_connection";

  QSqlDatabase db;

  if (QSqlDatabase::contains(connectionName))
  {
    db = QSqlDatabase::database(connectionName);
  }
  else
  {
    db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(dbPath);
  }

  db.open();

  if (!db.isOpen())
  {
    qWarning() << "Cannot open database" << db.lastError().text();
  }

  const QString createTableSql =
    "CREATE TABLE IF NOT EXISTS service_records ("
    "record_id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "event_type TEXT NOT NULL, "
    "name TEXT NOT NULL, "
    "notes TEXT, "
    "price REAL, "
    "mileage INTEGER, "
    "service_date TEXT, "
    "repeat_after_distance INTEGER, "
    "has_repeat_after_distance INTEGER DEFAULT 0, "
    "repeat_after_months INTEGER, "
    "has_repeat_after_months INTEGER DEFAULT 0 "
    ")";

  QSqlQuery query(db);
  if (!query.exec(createTableSql))
  {
    qWarning() << "Cannot create table " << query.lastError().text();
  }

  return db;
}

QSqlRecord ServiceRecordModel::sqlRecordFromServiceRecord(ServiceRecord* record) const
{
  QSqlRecord sqlRecord = m_model->record();

  QMetaEnum metaEnum = QMetaEnum::fromType<ServiceRecord::EventType>();

  sqlRecord.setValue("event_type", QString::fromUtf8(metaEnum.valueToKey(record->eventType())));
  sqlRecord.setValue("name", record->name());
  sqlRecord.setValue("notes", record->notes());
  sqlRecord.setValue("price", record->price());
  sqlRecord.setValue("mileage", record->mileage());
  sqlRecord.setValue("service_date", record->serviceDate().toString(Qt::ISODate));
  sqlRecord.setValue("repeat_after_distance", record->repeatAfterDistance());
  sqlRecord.setValue("has_repeat_after_distance", record->hasRepeatAfterDistance() ? 1 : 0);
  sqlRecord.setValue("repeat_after_months", record->repeatAfterMonths());
  sqlRecord.setValue("has_repeat_after_months", record->hasRepeatAfterMonths() ? 1 : 0);

  return sqlRecord;
}

ServiceRecord* ServiceRecordModel::serviceRecordFromSqlRecord(const QSqlRecord& record) const
{
  auto* serviceRecord = new ServiceRecord(const_cast<ServiceRecordModel*>(this));

  QString   eventTypeStr = record.value("event_type").toString();
  QMetaEnum metaEnum     = QMetaEnum::fromType<ServiceRecord::EventType>();
  bool      ok           = false;
  int       enumValue    = metaEnum.keyToValue(eventTypeStr.toUtf8().constData(), &ok);
  serviceRecord->setEventType(ok ? static_cast<ServiceRecord::EventType>(enumValue) : ServiceRecord::Maintenance);

  serviceRecord->setName(record.value("name").toString());
  serviceRecord->setNotes(record.value("notes").toString());
  serviceRecord->setPrice(record.value("price").toInt());
  serviceRecord->setMileage(record.value("mileage").toInt());

  auto date = QDate::fromString(record.value("service_date").toString(), Qt::ISODate);
  serviceRecord->setServiceDate(date);

  serviceRecord->setRepeatAfterDistance(record.value("repeat_after_distance").toInt());
  serviceRecord->setHasRepeatAfterDistance(record.value("has_repeat_after_distance").toBool());
  serviceRecord->setRepeatAfterMonths(record.value("repeat_after_months").toInt());
  serviceRecord->setHasRepeatAfterMonths(record.value("has_repeat_after_months").toBool());

  QQmlEngine::setObjectOwnership(serviceRecord, QQmlEngine::JavaScriptOwnership);
  return serviceRecord;
}
