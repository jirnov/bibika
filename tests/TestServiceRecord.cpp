#include <QSignalSpy>
#include <QTest>
#include <ServiceRecord.h>
#include <ServiceRecordBuilder.h>
#include <TestServiceRecord.h>

namespace {
const auto testName = "Entering orbit";
const auto testNotes = "Yuri Gagarin, the first man in space";
const auto testEventType = ServiceRecord::Maintenance;
const int testPrice = 15000;
const int testMileage = 40868;
const QDate testServiceDate = QDate::fromString("1961-04-12", Qt::ISODate);
const bool testHasRepeatAfterDistance = true;
const int testRepeatAfterDistance = 384400;
const bool testHasRepeatAfterMonths = true;
const int testRepeatAfterMonths = 99;
} // namespace

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

    record.setName(testName);
    record.setNotes(testNotes);
    record.setMileage(testMileage);
    record.setPrice(testPrice);
    record.setEventType(testEventType);
    record.setServiceDate(testServiceDate);
    record.setHasRepeatAfterDistance(testHasRepeatAfterDistance);
    record.setRepeatAfterDistance(testRepeatAfterDistance);
    record.setHasRepeatAfterMonths(testHasRepeatAfterMonths);
    record.setRepeatAfterMonths(testRepeatAfterMonths);

    QVERIFY(record.isValid());

    QCOMPARE(record.name(), testName);
    QCOMPARE(record.notes(), testNotes);
    QCOMPARE(record.mileage(), testMileage);
    QCOMPARE(record.serviceDate(), testServiceDate);
    QCOMPARE(record.eventType(), testEventType);
    QCOMPARE(record.hasRepeatAfterDistance(), testHasRepeatAfterDistance);
    QCOMPARE(record.repeatAfterDistance(), testRepeatAfterDistance);
    QCOMPARE(record.hasRepeatAfterMonths(), testHasRepeatAfterMonths);
    QCOMPARE(record.repeatAfterMonths(), testRepeatAfterMonths);
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

    record.setName(testName);
    QCOMPARE(nameSpy.count(), 1);

    record.setName(testName);
    QCOMPARE(nameSpy.count(), 1);

    record.setName("");
    QCOMPARE(nameSpy.count(), 2);

    record.setNotes(testNotes);
    QCOMPARE(notesSpy.count(), 1);

    record.setNotes(testNotes);
    QCOMPARE(notesSpy.count(), 1);

    record.setNotes("");
    QCOMPARE(notesSpy.count(), 2);

    record.setMileage(testMileage);
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
