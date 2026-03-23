#include <TestServiceRecordModel.h>

#include <ServiceRecordModel.h>
#include <QTest>
#include <QSqlError>
#include <ServiceRecord.h>

namespace
{
const auto testName  = "Entering orbit";
const auto testNotes = "Yuri Gagarin, the first man in space";
}  // namespace

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
  m_model->clear();

  QVERIFY(m_model->rowCount() == 0);
}

void TestServiceRecordModel::cleanup()
{
  delete m_model;
}

void TestServiceRecordModel::testAppendRecord()
{
  const auto eventType = ServiceRecord::Maintenance;

  QVERIFY(m_model->rowCount() == 0);

  ServiceRecord* serviceRecord = new ServiceRecord(this);
  serviceRecord->setName(testName);
  serviceRecord->setNotes(testNotes);
  serviceRecord->setEventType(eventType);

  m_model->append(serviceRecord);

  QVERIFY(m_model->rowCount() == 1);

  ServiceRecord* copyRec = m_model->getByIndex(0, this);

  QVERIFY(copyRec != nullptr);

  QCOMPARE(copyRec->name(), testName);
  QCOMPARE(copyRec->notes(), testNotes);
  QCOMPARE(copyRec->eventType(), eventType);
}

void TestServiceRecordModel::testUpdateRecord()
{
  const auto oldEventType = ServiceRecord::Maintenance;
  const auto newEventType = ServiceRecord::Service;

  int recordId = -1;

  // Добавляем пару записей для проверки
  {
    ServiceRecord* serviceRecord = new ServiceRecord(this);
    serviceRecord->setName(testName);
    serviceRecord->setNotes(testNotes);
    serviceRecord->setEventType(oldEventType);
    serviceRecord->setMileage(0);

    recordId = m_model->append(serviceRecord);
    QVERIFY(recordId != -1);

    serviceRecord->setName(testName + QString("2"));
    serviceRecord->setNotes(testNotes + QString("2"));
    serviceRecord->setMileage(2000);

    m_model->append(serviceRecord);
  }

  QVERIFY(m_model->rowCount() == 2);

  // Обновляем ей EventType и загружаем обратно в модель
  {
    ServiceRecord* serviceRecord = m_model->getById(recordId, this);

    QVERIFY(serviceRecord != nullptr);

    QCOMPARE(serviceRecord->name(), testName);
    QCOMPARE(serviceRecord->notes(), testNotes);
    QCOMPARE(serviceRecord->eventType(), oldEventType);

    serviceRecord->setEventType(newEventType);

    m_model->updateRecordById(recordId, serviceRecord);
  }

  // Проверяем что в нашей копии всё хорошо
  {
    ServiceRecord* serviceRecord = m_model->getById(recordId, this);
    QCOMPARE(serviceRecord->name(), testName);
    QCOMPARE(serviceRecord->notes(), testNotes);
    QCOMPARE(serviceRecord->eventType(), newEventType);
  }

  // Проверяем что выдаёт модель для каждого столбца
  {
    ServiceRecord* newRecord = m_model->getById(recordId, this);

    QCOMPARE(newRecord->name(), testName);
    QCOMPARE(newRecord->notes(), testNotes);
    QCOMPARE(newRecord->eventType(), newEventType);
  }
}
