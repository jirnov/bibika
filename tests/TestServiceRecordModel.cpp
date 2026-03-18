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

  ServiceRecord* copyRec = m_model->getByIndex(0);

  QVERIFY(copyRec != nullptr);

  QCOMPARE(copyRec->name(), testName);
  QCOMPARE(copyRec->notes(), testNotes);
  QCOMPARE(copyRec->eventType(), eventType);
}

void TestServiceRecordModel::testUpdateRecord()
{
  const auto oldEventType = ServiceRecord::Maintenance;
  const auto newEventType = ServiceRecord::Service;

  // Добавляем одну запись для проверки
  {
    ServiceRecord* serviceRecord = new ServiceRecord(this);
    serviceRecord->setName(testName);
    serviceRecord->setNotes(testNotes);
    serviceRecord->setEventType(oldEventType);

    m_model->append(serviceRecord);
  }

  QVERIFY(m_model->rowCount() == 1);

  const int recordId = m_model->data(m_model->index(0, 0), ServiceRecordModel::RecordIdRole).toInt();

  QVERIFY(recordId != 0);

  // Обновляем ей EventType и загружаем обратно в модель
  {
    ServiceRecord* serviceRecord = m_model->getByIndex(0);

    QVERIFY(serviceRecord != nullptr);

    QCOMPARE(serviceRecord->name(), testName);
    QCOMPARE(serviceRecord->notes(), testNotes);
    QCOMPARE(serviceRecord->eventType(), oldEventType);

    serviceRecord->setEventType(newEventType);

    m_model->updateRecordById(recordId, serviceRecord);
  }

  // Проверяем что в нашей копии всё хорошо
  {
    ServiceRecord* serviceRecord = m_model->getById(recordId);
    QCOMPARE(serviceRecord->name(), testName);
    QCOMPARE(serviceRecord->notes(), testNotes);
    QCOMPARE(serviceRecord->eventType(), newEventType);
  }

  // Проверяем что выдаёт модель для каждого столбца
  {
    QCOMPARE(m_model->data(m_model->index(0, 0), ServiceRecordModel::RecordIdRole), recordId);
    QCOMPARE(m_model->data(m_model->index(0, 0), ServiceRecordModel::NameRole).toString(), testName);
    QCOMPARE(m_model->data(m_model->index(0, 0), ServiceRecordModel::NotesRole).toString(), testNotes);
    const int eventType    = m_model->data(m_model->index(0, 0), ServiceRecordModel::EventTypeRole).toInt();
    const int expectedType = static_cast<int>(newEventType);
    QCOMPARE(eventType, expectedType);
  }
}
