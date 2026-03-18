#include <TestServiceRecord.h>

#include <ServiceRecordBuilder.h>

namespace
{
const auto  name                   = "TestName";
const auto  notes                  = "TestNotes";
const auto  eventType              = ServiceRecord::Maintenance;
const int   price                  = 20000;
const int   mileage                = 500000;
const QDate serviceDate            = QDate::currentDate();
const bool  hasRepeatAfterDistance = true;
const int   repeatAfterDistance    = 15000;
const bool  hasRepeatAfterMonths   = true;
const int   repeatAfterMonths      = 12;
}  // namespace

void TestServiceRecord::init()
{
  qDebug() << "Starting test...";
}

void TestServiceRecord::cleanup()
{
  qDebug() << "Test finished";
}

void TestServiceRecord::testDefaultConstructor()
{
  ServiceRecord record;

  // Проверяем значения по умолчанию
  QCOMPARE(record.name(), QString());
  QCOMPARE(record.notes(), QString());
  QCOMPARE(record.price(), 0.0);
  QCOMPARE(record.mileage(), 0);
  QCOMPARE(record.serviceDate(), QDate::currentDate());
  QCOMPARE(record.eventType(), ServiceRecord::Maintenance);
  QCOMPARE(record.repeatAfterDistance(), 0);
  QCOMPARE(record.hasRepeatAfterDistance(), false);
  QCOMPARE(record.repeatAfterMonths(), 0);
  QCOMPARE(record.hasRepeatAfterMonths(), false);
}

void TestServiceRecord::testSettersAndGetters()
{
  ServiceRecord record;

  record.setName(name);
  record.setNotes(notes);
  record.setMileage(mileage);
  record.setPrice(price);
  record.setEventType(eventType);
  record.setHasRepeatAfterDistance(hasRepeatAfterDistance);
  record.setRepeatAfterDistance(repeatAfterDistance);
  record.setHasRepeatAfterMonths(hasRepeatAfterMonths);
  record.setRepeatAfterMonths(repeatAfterMonths);

  QVERIFY(record.isValid());

  QCOMPARE(record.name(), name);
  QCOMPARE(record.notes(), notes);
  QCOMPARE(record.mileage(), mileage);
  QCOMPARE(record.serviceDate(), serviceDate);
  QCOMPARE(record.eventType(), eventType);
  QCOMPARE(record.hasRepeatAfterDistance(), hasRepeatAfterDistance);
  QCOMPARE(record.repeatAfterDistance(), repeatAfterDistance);
  QCOMPARE(record.hasRepeatAfterMonths(), hasRepeatAfterMonths);
  QCOMPARE(record.repeatAfterMonths(), repeatAfterMonths);
}

void TestServiceRecord::testSignals()
{
  ServiceRecord record;

  QSignalSpy nameSpy(&record, &ServiceRecord::nameChanged);
  QSignalSpy notesSpy(&record, &ServiceRecord::notesChanged);
  QSignalSpy priceSpy(&record, &ServiceRecord::priceChanged);
  QSignalSpy mileageSpy(&record, &ServiceRecord::mileageChanged);
  QSignalSpy serviceDateSpy(&record, &ServiceRecord::serviceDateChanged);
  QSignalSpy eventTypeSpy(&record, &ServiceRecord::eventTypeChanged);
  QSignalSpy repeatDistanceSpy(&record, &ServiceRecord::repeatAfterDistanceChanged);
  QSignalSpy hasRepeatDistanceSpy(&record, &ServiceRecord::hasRepeatAfterDistanceChanged);
  QSignalSpy repeatMonthsSpy(&record, &ServiceRecord::repeatAfterMonthsChanged);
  QSignalSpy hasRepeatMonthsSpy(&record, &ServiceRecord::hasRepeatAfterMonthsChanged);

  QCOMPARE(nameSpy.count(), 0);
  QCOMPARE(notesSpy.count(), 0);
  QCOMPARE(priceSpy.count(), 0);
  QCOMPARE(mileageSpy.count(), 0);
  QCOMPARE(serviceDateSpy.count(), 0);
  QCOMPARE(eventTypeSpy.count(), 0);
  QCOMPARE(repeatDistanceSpy.count(), 0);
  QCOMPARE(hasRepeatDistanceSpy.count(), 0);
  QCOMPARE(repeatMonthsSpy.count(), 0);
  QCOMPARE(hasRepeatMonthsSpy.count(), 0);

  record.setName(name);
  QCOMPARE(nameSpy.count(), 1);

  record.setName(name);
  QCOMPARE(nameSpy.count(), 1);

  record.setName("");
  QCOMPARE(nameSpy.count(), 2);

  record.setNotes(notes);
  QCOMPARE(notesSpy.count(), 1);

  record.setNotes(notes);
  QCOMPARE(notesSpy.count(), 1);

  record.setNotes("");
  QCOMPARE(notesSpy.count(), 2);

  record.setMileage(mileage);
  QCOMPARE(mileageSpy.count(), 1);
  QCOMPARE(serviceDateSpy.count(), 0);

  record.setServiceDate(QDate(1, 1, 1));
  QCOMPARE(serviceDateSpy.count(), 1);

  record.setEventType(ServiceRecord::Repair);
  QCOMPARE(eventTypeSpy.count(), 1);
  record.setEventType(ServiceRecord::Maintenance);
  QCOMPARE(eventTypeSpy.count(), 2);
  record.setEventType(ServiceRecord::Service);
  QCOMPARE(eventTypeSpy.count(), 3);

  record.setHasRepeatAfterDistance(true);
  QCOMPARE(hasRepeatDistanceSpy.count(), 1);
  record.setHasRepeatAfterDistance(false);
  QCOMPARE(hasRepeatDistanceSpy.count(), 2);

  record.setHasRepeatAfterMonths(true);
  QCOMPARE(hasRepeatMonthsSpy.count(), 1);
  record.setHasRepeatAfterMonths(false);
  QCOMPARE(hasRepeatMonthsSpy.count(), 2);

  record.setRepeatAfterDistance(15000);
  QCOMPARE(repeatDistanceSpy.count(), 1);
  record.setRepeatAfterDistance(0);
  QCOMPARE(repeatDistanceSpy.count(), 2);

  record.setRepeatAfterMonths(12);
  QCOMPARE(repeatMonthsSpy.count(), 1);
  record.setRepeatAfterMonths(0);
  QCOMPARE(repeatMonthsSpy.count(), 2);
}

void TestServiceRecord::testJson()
{
  ServiceRecord record;

  record.setName(name);
  record.setNotes(notes);
  record.setEventType(eventType);
  record.setPrice(price);
  record.setMileage(mileage);
  record.setServiceDate(serviceDate);

  const auto jsonString = record.toJSON();

  ServiceRecord* restored = ServiceRecordBuilder::fromJSON(jsonString, this);

  QCOMPARE(restored->name(), name);
  QCOMPARE(restored->notes(), notes);
  QCOMPARE(restored->price(), price);
  QCOMPARE(restored->eventType(), eventType);
  QCOMPARE(restored->mileage(), mileage);
  QCOMPARE(restored->serviceDate(), serviceDate);
}
