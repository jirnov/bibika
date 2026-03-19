#ifndef TESTCARINFOBUILDER_H
#define TESTCARINFOBUILDER_H

#include <QObject>
#include <CarInfoBuilder.h>

class TestCarInfoBuilder : public QObject
{
  Q_OBJECT

 private slots:
  void initTestCase();  // Выполняется перед всеми тестами

  // Тесты createEmpty
  void testCreateEmpty();
  void testCreateEmptyWithParent();

  // Тесты fromJSON
  void testFromJSON_ValidFull();
  void testFromJSON_ValidPartial();
  void testFromJSON_InvalidJson();
  void testFromJSON_EmptyJson();
  void testFromJSON_ExtraFields();
  void testFromJSON_InvalidTypes();
  void testFromJSON_WithParent();

  // Тесты toJSON
  void testToJSON_CompleteCar();
  void testToJSON_EmptyCar();
  void testToJSON_PartialCar();
  void testToJSON_SpecialCharacters();
  void testToJSON_ExtremeValues();

  // Интеграционные тесты
  void testFromJSON_WithTimeInDate();
  void testFromJSON_WithInvalidDate();
  void testSerializationRoundTrip();
  void testMultipleSerializations();
  void testNullPointer();

 private:
  CarInfoBuilder* builder;
  QString         validJsonFull;
  QString         validJsonPartial;
};

#endif  // TESTCARINFOBUILDER_H
