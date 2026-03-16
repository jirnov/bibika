#include "ServiceRecordModel.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QTimer>
#include <QStandardPaths>
#include <QSqlRecord>
#include <QFileInfo>
#include <QDir>
#include "ServiceRecord.h"


ServiceRecordModel::ServiceRecordModel(QObject* parent) : QAbstractListModel{ parent }
{
  QSqlDatabase db = openDatabase();

  m_model = new QSqlTableModel(this, db);
  m_model->setTable("service_records");
  m_model->setSort(m_model->fieldIndex("mileage"), Qt::AscendingOrder);
  m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  if (!m_model->select()) {
    qWarning() << "Failed to select data" << m_model->lastError().text();
  }
}

int ServiceRecordModel::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid() || !m_model) {
    return 0;
  }
  return m_model->rowCount();
}

QVariant ServiceRecordModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid() || !m_model) {
    return {};
  }

  const QSqlRecord record = m_model->record(index.row());

  switch (role) {
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
  case RepeatAfterDistanceRole:
    return record.value("repeat_after_distance");
  case HasRepeatAfterDistanceRole:
    return record.value("has_repeat_after_distance").toBool();
  case RepeatAfterMonthsRole:
    return record.value("repeat_after_months");
  case HasRepeatAfterMonthsRole:
    return record.value("has_repeat_after_months").toBool();
  case IsValidRole:
    return !record.value("name").toString().isEmpty();
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
  roles[IsValidRole]                = "isValid";
  return roles;
}

void ServiceRecordModel::append(ServiceRecord* record)
{
  if (!record || !m_model) {
    return;
  }

  QSqlRecord sqlRecord = recordFromServiceRecord(record);

  int newRow = m_model->rowCount();
  beginInsertRows(QModelIndex(), newRow, newRow);
  bool success = m_model->insertRecord(newRow, sqlRecord);

  if (success) {
    if (m_model->submitAll()) {
      qDebug() << "Record appended sucessfully";
    }
    else {
      qWarning() << "Failed to submit append:" << m_model->lastError().text();
      m_model->revertAll();
      m_model->select();
    }
  }
  else {
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

  if (success) {
    if (m_model->submitAll()) {
      qDebug() << "Clear database sucessfullly";
    }
    else {
      qWarning() << "Failed to submit clear: " << m_model->lastError().text();
    }
  }
  else {
    qWarning() << "Failed to clear: " << m_model->lastError().text();
  }

  endResetModel();
}

void ServiceRecordModel::removeRecordById(int recordId)
{
  if (!m_model || recordId < 0) {
    return;
  }

  QSqlQuery query(m_model->database());
  query.prepare("DELETE FROM service_records WHERE record_id = :id");
  query.bindValue(":id", recordId);

  if (query.exec()) {
    m_model->select();
    qDebug() << "Record" << recordId << "deleted sucessfully";

    beginResetModel();
    m_model->select();
    endResetModel();
  }
  else {
    qWarning() << "Delete failed:" << query.lastError().text();
  }
}

QSqlDatabase ServiceRecordModel::openDatabase()
{
  const QString dbPath =
    QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/bibika_service.sqlite";
  const QString connectionName = "service_records_connection";

  QSqlDatabase db;

  if (QSqlDatabase::contains(connectionName)) {
    db = QSqlDatabase::database(connectionName);
  }
  else {
    db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(dbPath);
  }

  db.open();

  if (!db.isOpen()) {
    qWarning() << "Cannot open database" << db.lastError().text();
  }

  const QString createTableSql =
    "CREATE TABLE IF NOT EXISTS service_records ("
    "record_id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "event_type INTEGER NOT NULL, "
    "name TEXT NOT NULL, "
    "notes TEXT, "
    "price REAL, "
    "mileage INTEGER, "
    "service_date TEXT, "
    "repeat_after_distance INTEGER, "
    "has_repeat_after_distance INTEGER DEFAULT 0, "
    "repeat_after_months INTEGER, "
    "has_repeat_after_months INTEGER DEFAULT 0"
    ")";

  QSqlQuery query(db);
  if (!query.exec(createTableSql)) {
    qWarning() << "Cannot create table " << query.lastError().text();
  }

  return db;
}

QSqlRecord ServiceRecordModel::recordFromServiceRecord(ServiceRecord* record) const
{
  QSqlRecord sqlRecord = m_model->record();

  sqlRecord.setValue("event_type", record->eventType());
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
