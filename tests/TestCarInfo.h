#ifndef TESTCARINFO_H
#define TESTCARINFO_H

#include <QObject>
#include <QTest>

class CarInfo;

class TestCarInfo : public QObject
{
  Q_OBJECT

 private slots:
  void init();     // Выполняется перед каждым тестом
  void cleanup();  // Выполняется после каждого теста

  // Тесты конструктора и инициализации
  void testConstructor();
  void testDefaultValues();

  // Тесты геттеров/сеттеров
  void testSetGetBrandName();
  void testSetGetModelName();
  void testSetGetMileage();
  void testSetGetMileageUpdateDate();

  // Тесты сигналов
  void testBrandNameSignal();
  void testModelNameSignal();
  void testMileageSignal();
  void testMileageUpdateDateSignal();
  void testNameSignal();
  void testIsValidSignal();

  // Тесты валидации
  void testValidateBrand();
  void testValidateModel();
  void testValidateAll();

  // Тесты вычисляемых свойств
  void testName();
  void testIsValid();

  // Тесты сложных сценариев
  void testMultipleUpdates();
  void testIsValidTransitions();
  void testNoSignalsOnSameValue();

  // Тесты граничных значений
  void testEdgeCases();

  void testInvariants();

 private:
  CarInfo* car;
};

#endif  // TESTCARINFO_H