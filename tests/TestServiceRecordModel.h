#ifndef TESTSERVICERECORDMODEL_H
#define TESTSERVICERECORDMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <ServiceRecordModel.h>

class TestServiceRecordModel : public QObject {
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testAppendRecord();
    void testUpdateRecord();

private:
    ServiceRecordModel* m_model;
};

#endif // TESTSERVICERECORDMODEL_H
