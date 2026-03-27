#pragma once

#include <QAbstractListModel>
#include <ServiceRecordRepository.h>
#include <memory>

class ServiceRecord;

// Содержит все сервисные события
// хранение в базе данных SQLite с помощью QSqlTableModel
class ServiceRecordModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        RecordIdRole = Qt::UserRole,
        EventTypeRole,
        NameRole,
        NotesRole,
        PriceRole,
        MileageRole,
        ServiceDateRole,
        ParentRecordIdRole,
        RepeatAfterDistanceRole,
        HasRepeatAfterDistanceRole,
        RepeatAfterMonthsRole,
        HasRepeatAfterMonthsRole
    };

    explicit ServiceRecordModel(QObject* parent = nullptr);
    ServiceRecordModel(std::unique_ptr<ServiceRecordRepository> &&repo, QObject *parent);

public:
    // QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE int append(ServiceRecord* record);
    Q_INVOKABLE bool clear();
    Q_INVOKABLE void remove(int recordId);
    Q_INVOKABLE ServiceRecord* find(int recordId, QObject* parent) const;
    Q_INVOKABLE int count() const;

    Q_INVOKABLE bool update(int recordId, ServiceRecord* updateRecord);

private:
    std::unique_ptr<ServiceRecordRepository> m_repo;
};
