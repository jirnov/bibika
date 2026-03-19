#ifndef SERVICERECORDMODEL_H
#define SERVICERECORDMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include <QSqlTableModel>

class ServiceRecord;

// Содержит все сервисные события
// хранение в базе данных SQLite с помощью QSqlTableModel
class ServiceRecordModel : public QAbstractListModel
{
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON


 public:
  enum Roles
  {
    RecordIdRole = Qt::UserRole,
    EventTypeRole,
    NameRole,
    NotesRole,
    PriceRole,
    MileageRole,
    ServiceDateRole,
    RepeatAfterDistanceRole,
    HasRepeatAfterDistanceRole,
    RepeatAfterMonthsRole,
    HasRepeatAfterMonthsRole
  };

  ServiceRecordModel(const QSqlDatabase& db = QSqlDatabase(), QObject* parent = nullptr);

 public:
  // QAbstractListModel
  int                    rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant               data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE int            append(ServiceRecord* record);
  Q_INVOKABLE void           clear();
  Q_INVOKABLE void           removeById(int recordId);
  Q_INVOKABLE ServiceRecord* getById(int recordId) const;
  Q_INVOKABLE int            count() const;

  Q_INVOKABLE void updateRecordById(int recordId, ServiceRecord* updateRecord);

  ServiceRecord* getByIndex(int index) const;

 private:
  void               removeByIndex(int index);
  std::optional<int> indexById(int recordId) const;

  QSqlDatabase   openDatabase();
  void           createTableIfNotExists(const QSqlDatabase& db);
  QSqlRecord     sqlRecordFromServiceRecord(ServiceRecord* record) const;
  ServiceRecord* serviceRecordFromSqlRecord(const QSqlRecord& record) const;

  QSqlTableModel* m_model = nullptr;
};


#endif  // SERVICERECORDMODEL_H
