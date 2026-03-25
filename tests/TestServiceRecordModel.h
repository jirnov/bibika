#ifndef TESTSERVICERECORDMODEL_H
#define TESTSERVICERECORDMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <ServiceRecordModel.h>

class TestServiceRecordModel : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();

    void init();
    void cleanup();

    void testAppendRecord();
    void testUpdateRecord();

private:
    QSqlDatabase m_db;
    ServiceRecordModel* m_model{nullptr};
};

#endif // TESTSERVICERECORDMODEL_H
