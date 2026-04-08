#include "ServiceRecordModel.h"
#include "ServiceRecord.h"

namespace {
constexpr QStringView tableName = u"service_records";
}

ServiceRecordModel::ServiceRecordModel(QObject* parent)
   : QAbstractListModel{parent}
{}

ServiceRecordModel::ServiceRecordModel(std::unique_ptr<ServiceRecordRepository>&& repo, QObject* parent)
   : QAbstractListModel{parent}
   , m_repo{std::move(repo)}
{
    if (!m_repo)
    {
        qWarning() << "Invalid repository";
        return;
    }

    connect(m_repo.get(), &ServiceRecordRepository::beginRemoveRow, this, [&](auto row) {
        beginRemoveRows(QModelIndex(), row, row);
    });
    connect(m_repo.get(), &ServiceRecordRepository::endRemoveRow, this, &ServiceRecordModel::endRemoveRows);

    connect(m_repo.get(), &ServiceRecordRepository::beginInsertRow, this, [&](auto row) {
        beginInsertRows(QModelIndex(), row, row);
    });
    connect(m_repo.get(), &ServiceRecordRepository::endInsertRow, this, &ServiceRecordModel::endInsertRows);

    connect(m_repo.get(), &ServiceRecordRepository::dataChanged, this, [&](auto row) {
        emit dataChanged(index(row), index(row));
    });

    connect(m_repo.get(), &ServiceRecordRepository::beginResetModel, this, &ServiceRecordModel::beginResetModel);
    connect(m_repo.get(), &ServiceRecordRepository::endResetModel, this, &ServiceRecordModel::endResetModel);
}

int ServiceRecordModel::rowCount(const QModelIndex& parent) const
{
    return m_repo->count();
}

QVariant ServiceRecordModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    static const QHash<int, QString> roles = {
       {RecordIdRole, "record_id"},
       {EventTypeRole, "event_type"},
       {NameRole, "name"},
       {NotesRole, "notes"},
       {PriceRole, "price"},
       {MileageRole, "mileage"},
       {ServiceDateRole, "service_date"},
       {ParentRecordIdRole, "parent_record_id"},
       {RepeatAfterDistanceRole, "repeat_after_distance"},
       {HasRepeatAfterDistanceRole, "has_repeat_after_distance"},
       {RepeatAfterMonthsRole, "repeat_after_months"},
       {HasRepeatAfterMonthsRole, "has_repeat_after_months"},
    };

    return m_repo->data(index.row(), roles.value(role));
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
    return m_repo->append(*record).value_or(-1);
}

bool ServiceRecordModel::clear()
{
    return m_repo->clear();
}

void ServiceRecordModel::remove(int recordId)
{
    m_repo->remove(recordId);
}

ServiceRecord* ServiceRecordModel::find(int recordId, QObject* parent) const
{
    return m_repo->find(recordId, parent);
}

int ServiceRecordModel::count() const
{
    return m_repo->count();
}

bool ServiceRecordModel::update(int recordId, ServiceRecord* sr)
{
    return m_repo->update(recordId, *sr);
}
