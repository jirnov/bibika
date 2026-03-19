#include "ServiceRecordModel.h"
#include "ServiceRecord.h"
#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStandardPaths>
#include <ServiceRecordBuilder.h>

ServiceRecordModel::ServiceRecordModel(const QSqlDatabase& db, QObject* parent) : QAbstractListModel{ parent }
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
  const auto& toEventType = ServiceRecordBuilder::str2EventType;

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
    return QVariant::fromValue(toEventType(record.value("event_type").toString()));
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
  if (m_model->rowCount() == 0)
  {
    return;
  }
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

int ServiceRecordModel::count() const
{
  return rowCount();
}

void ServiceRecordModel::updateRecordById(int recordId, ServiceRecord* sr)
{
  const auto& toString = ServiceRecordBuilder::eventType2Str;

  if (auto index = indexById(recordId); index.has_value())
  {
    QSqlRecord record = m_model->record(index.value());
    record.setValue("event_type", toString(sr->eventType()));
    record.setValue("name", sr->name());
    record.setValue("notes", sr->notes());
    record.setValue("mileage", sr->mileage());
    record.setValue("service_date", sr->serviceDate().toString(Qt::ISODate));
    record.setValue("repeat_after_distance", sr->repeatAfterDistance());
    record.setValue("has_repeat_after_distance", sr->hasRepeatAfterDistance());
    record.setValue("repeat_after_months", sr->repeatAfterMonths());
    record.setValue("has_repeat_after_months", sr->hasRepeatAfterMonths());

    if (m_model->setRecord(index.value(), record))
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
    if (!db.isValid())
    {
      qWarning() << "Cannot add database" << db.lastError().text();
    }
    db.setDatabaseName(dbPath);
  }

  db.open();

  if (!db.isOpen())
  {
    qWarning() << "Cannot open database" << db.lastError().text();
  }

  return db;
}

void ServiceRecordModel::createTableIfNotExists(const QSqlDatabase& db)
{
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

ServiceRecord* ServiceRecordModel::serviceRecordFromSqlRecord(const QSqlRecord& record) const
{
  const auto& toEventType = ServiceRecordBuilder::str2EventType;

  auto* sr = new ServiceRecord(const_cast<ServiceRecordModel*>(this));

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

  QQmlEngine::setObjectOwnership(sr, QQmlEngine::JavaScriptOwnership);
  return sr;
}
