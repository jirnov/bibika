#include "ServiceRecordProxy.h"
#include "ServiceRecordModel.h"

ServiceRecordProxy::ServiceRecordProxy(QAbstractListModel* model, QObject* parent)
   : QSortFilterProxyModel(parent)
{
    setSourceModel(model);
    sort(0, Qt::AscendingOrder);
}

bool ServiceRecordProxy::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
    int leftValue = sourceModel()->data(left, ServiceRecordModel::MileageRole).toInt();
    int rightValue = sourceModel()->data(right, ServiceRecordModel::MileageRole).toInt();
    return leftValue < rightValue;
}
