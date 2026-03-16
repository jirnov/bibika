#ifndef SERVICERECORDMODEL_H
#define SERVICERECORDMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include <QSqlTableModel>

class ServiceRecord;

class ServiceRecordModel : public QAbstractListModel
{
  Q_OBJECT
  QML_ELEMENT

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
    HasRepeatAfterMonthsRole,
    IsValidRole,
  };

 public:
  explicit ServiceRecordModel(QObject* parent = nullptr);

 public:
  // QAbstractListModel
  int                    rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant               data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void append(ServiceRecord* record);
  Q_INVOKABLE void clear();
  Q_INVOKABLE void removeRecordById(int recordId);

 private:
  QSqlDatabase openDatabase();
  QSqlRecord   recordFromServiceRecord(ServiceRecord* record) const;

  QSqlTableModel* m_model = nullptr;
};

#endif  // SERVICERECORDMODEL_H
