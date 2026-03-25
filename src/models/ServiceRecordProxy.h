#ifndef SERVICERECORDPROXY_H
#define SERVICERECORDPROXY_H

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

class ServiceRecordProxy : public QSortFilterProxyModel {
    Q_OBJECT

public:
    ServiceRecordProxy(QAbstractListModel* model, QObject* parent = nullptr);

protected:
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
};

#endif // SERVICERECORDPROXY_H
