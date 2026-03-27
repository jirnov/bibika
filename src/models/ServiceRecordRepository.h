#pragma once

#include <QSqlDatabase>
#include <QSqlTableModel>

class ServiceRecord;

class ServiceRecordRepository : public QObject {
    Q_OBJECT

public:
    explicit ServiceRecordRepository(const QSqlDatabase &db, QObject *parent=nullptr);
    ~ServiceRecordRepository() = default;

    ServiceRecord* find(int id, QObject *parent) const;

    QVariantHash getByIndex(int index) const;

    std::optional<int> append(const ServiceRecord &newRecord);
    bool update(int id, const ServiceRecord &record);
    bool remove(int id);
    int count() const;
    bool clear();

signals:
    void beginInsertRow(int row);
    void endInsertRow();

    void beginRemoveRow(int row);
    void endRemoveRow();

    void beginResetModel();
    void endResetModel();

    void dataChanged(int row);

private:
    std::optional<int> indexById(int id) const;

    QSqlRecord toSqlRecord(const ServiceRecord& record) const;
    ServiceRecord* fromSqlRecord(const QSqlRecord& record, QObject* parent) const;

    void createTableIfNotExists(const QSqlDatabase& db);

    std::unique_ptr<QSqlTableModel> m_model;
};
