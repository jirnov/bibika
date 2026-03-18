#include <TestServiceRecordModel.h>

#include <ServiceRecordModel.h>
#include <QTest>
#include <QSqlError>

void TestServiceRecordModel::initTestCase()
{
  if (QSqlDatabase::contains("test_connection"))
  {
    QSqlDatabase::removeDatabase("test_connection");
  }

  m_db = QSqlDatabase::addDatabase("QSQLITE", "test_connection");
  QVERIFY(m_db.isValid());

  m_db.setDatabaseName(":memory:");
  QVERIFY(m_db.open());
}

void TestServiceRecordModel::init()
{
  m_model = new ServiceRecordModel(m_db, this);
}

void TestServiceRecordModel::cleanup()
{
  delete m_model;
}

void TestServiceRecordModel::testDefaultValues()
{
  QVERIFY(m_model->rowCount() == 0);
}
