// TestCarInfo.cpp
#include "TestCarInfo.h"
#include "CarInfo.h"
#include <QSignalSpy>
#include <QScopedPointer>
#include <limits>

void TestCarInfo::init()
{
  car = new CarInfo(this);
}

//=============================================================================
// Тесты конструктора и инициализации
//=============================================================================

void TestCarInfo::testConstructor()
{
  CarInfo carWithParent(this);
  QCOMPARE(carWithParent.parent(), this);

  CarInfo carWithoutParent;
  QVERIFY(carWithoutParent.parent() == nullptr);
}

void TestCarInfo::testDefaultValues()
{
  QCOMPARE(car->brandName(), QString());
  QCOMPARE(car->modelName(), QString());
  QCOMPARE(car->mileage(), 0);
  QVERIFY(!car->mileageUpdateDate().isValid());
  QCOMPARE(car->mileageUpdateDate(), QDate());
  QCOMPARE(car->name(), QString());
  QVERIFY(!car->isValid());
}

//=============================================================================
// Тесты геттеров/сеттеров
//=============================================================================

void TestCarInfo::testSetGetBrandName()
{
  QString testBrand = "SEAT";
  car->setBrandName(testBrand);
  QCOMPARE(car->brandName(), testBrand);

  // Пустая строка
  car->setBrandName("");
  QCOMPARE(car->brandName(), "");

  // Строка с пробелами
  car->setBrandName("  CUPRA  ");
  QCOMPARE(car->brandName(), "  CUPRA  ");

  car->setBrandName("Cupra");
  QCOMPARE(car->brandName(), "Cupra");

  // Информация невалидна без modelName
  QVERIFY(!car->isValid());
}

void TestCarInfo::testSetGetModelName()
{
  QString testModel = "Leon";
  car->setModelName(testModel);
  QCOMPARE(car->modelName(), testModel);

  // Пустая строка
  car->setModelName("");
  QCOMPARE(car->modelName(), "");

  // Другая модель
  car->setModelName("Formentor");
  QCOMPARE(car->modelName(), "Formentor");

  // Информация невалидна без modelName
  QVERIFY(!car->isValid());
}

void TestCarInfo::testSetGetMileage()
{
  const int testMileage = 15000;
  car->setMileage(testMileage);
  QCOMPARE(car->mileage(), testMileage);

  // Ноль
  car->setMileage(0);
  QCOMPARE(car->mileage(), 0);

  // Отрицательный пробег приводит к нулю
  car->setMileage(-1000);
  QCOMPARE(car->mileage(), 0);

  // Максимальное значение
  car->setMileage(std::numeric_limits<int>::max());
  QCOMPARE(car->mileage(), std::numeric_limits<int>::max());

  // Минимальное значение
  car->setMileage(std::numeric_limits<int>::min());
  QCOMPARE(car->mileage(), 0);
}

void TestCarInfo::testSetGetMileageUpdateDate()
{
  QDate testDate(2026, 3, 20);
  car->setMileageUpdateDate(testDate);
  QCOMPARE(car->mileageUpdateDate(), testDate);

  // Будущая дата
  QDate futureDate(2030, 12, 31);
  car->setMileageUpdateDate(futureDate);
  QCOMPARE(car->mileageUpdateDate(), futureDate);

  // Прошлая дата
  QDate pastDate(2000, 1, 1);
  car->setMileageUpdateDate(pastDate);
  QCOMPARE(car->mileageUpdateDate(), pastDate);

  // Невалидная дата
  QDate invalidDate;
  car->setMileageUpdateDate(invalidDate);
  QVERIFY(!car->mileageUpdateDate().isValid());
}

//=============================================================================
// Тесты сигналов
//=============================================================================

void TestCarInfo::testBrandNameSignal()
{
  QSignalSpy spy(car, &CarInfo::brandNameChanged);

  car->setBrandName("SEAT");
  QCOMPARE(spy.count(), 1);

  car->setBrandName("Cupra");
  QCOMPARE(spy.count(), 2);

  // Повторная установка того же значения
  car->setBrandName("Cupra");
  QCOMPARE(spy.count(), 2);  // Сигнал не должен испускаться
}

void TestCarInfo::testModelNameSignal()
{
  QSignalSpy spy(car, &CarInfo::modelNameChanged);

  car->setModelName("Leon");
  QCOMPARE(spy.count(), 1);

  car->setModelName("Arona");
  QCOMPARE(spy.count(), 2);

  car->setModelName("Arona");
  QCOMPARE(spy.count(), 2);
}

void TestCarInfo::testMileageSignal()
{
  QSignalSpy spy(car, &CarInfo::mileageChanged);

  car->setMileage(1000);
  QCOMPARE(spy.count(), 1);

  car->setMileage(2000);
  QCOMPARE(spy.count(), 2);

  car->setMileage(2000);
  QCOMPARE(spy.count(), 2);
}

void TestCarInfo::testMileageUpdateDateSignal()
{
  QSignalSpy spy(car, &CarInfo::mileageUpdateDateChanged);

  QDate date1(2026, 3, 20);
  car->setMileageUpdateDate(date1);
  QCOMPARE(spy.count(), 1);

  QDate date2(2026, 3, 21);
  car->setMileageUpdateDate(date2);
  QCOMPARE(spy.count(), 2);

  car->setMileageUpdateDate(date2);
  QCOMPARE(spy.count(), 2);
}

void TestCarInfo::testNameSignal()
{
  QSignalSpy spy(car, &CarInfo::nameChanged);

  // nameChanged должен испускаться при изменении brand или model
  car->setBrandName("SEAT");
  QCOMPARE(spy.count(), 1);

  car->setModelName("Leon");
  QCOMPARE(spy.count(), 2);

  car->setBrandName("Cupra");
  QCOMPARE(spy.count(), 3);

  car->setModelName("Formentor");
  QCOMPARE(spy.count(), 4);

  // Проверяем, что сигнал не испускается при установке тех же значений
  car->setBrandName("Cupra");
  QCOMPARE(spy.count(), 4);

  // Проверяем формирование полного имени
  QCOMPARE(car->name(), "Cupra Formentor");
}

void TestCarInfo::testIsValidSignal()
{
  QSignalSpy spy(car, &CarInfo::isValidChanged);

  // Изначально невалидно
  QVERIFY(!car->isValid());

  // Устанавливаем бренд - все еще невалидно
  car->setBrandName("SEAT");
  QCOMPARE(spy.count(), 0);  // isValid не изменился (было false, стало false)

  // Устанавливаем модель - становится валидно
  car->setModelName("Leon");
  QCOMPARE(spy.count(), 1);
  QVERIFY(car->isValid());

  // Меняем бренд - валидность сохраняется (true -> true)
  car->setBrandName("Cupra");
  QCOMPARE(spy.count(), 1);  // Сигнал не должен испускаться

  // Очищаем модель - становится невалидно
  car->setModelName("");
  QCOMPARE(spy.count(), 2);
  QVERIFY(!car->isValid());

  // Заполняем модель - снова валидно
  car->setModelName("Ateca");
  QCOMPARE(spy.count(), 3);
  QVERIFY(car->isValid());
}

//=============================================================================
// Тесты валидации
//=============================================================================

void TestCarInfo::testValidateBrand()
{
  // Пустой бренд
  car->setBrandName("");
  QVERIFY(!car->validateBrand().isEmpty());

  // Непустой бренд
  car->setBrandName("SEAT");
  QVERIFY(car->validateBrand().isEmpty());

  // Бренд с пробелами
  car->setBrandName("   ");
  QVERIFY(!car->validateBrand().isEmpty());

  // Другой валидный бренд
  car->setBrandName("Cupra");
  QVERIFY(car->validateBrand().isEmpty());
}

void TestCarInfo::testValidateModel()
{
  // Пустая модель
  car->setModelName("");
  QVERIFY(!car->validateModel().isEmpty());

  // Непустая модель
  car->setModelName("Leon");
  QVERIFY(car->validateModel().isEmpty());

  // Модель из пробелов невалидна
  car->setModelName("   ");
  QVERIFY(!car->validateModel().isEmpty());

  // Другая валидная модель
  car->setModelName("Formentor");
  QVERIFY(car->validateModel().isEmpty());
}

void TestCarInfo::testValidateAll()
{
  // Оба поля пустые
  auto errors = car->validateAll();
  QCOMPARE(errors.size(), 2);
  QVERIFY(errors.contains("brandName"));
  QVERIFY(errors.contains("modelName"));

  // Только бренд заполнен
  car->setBrandName("SEAT");
  errors = car->validateAll();
  QCOMPARE(errors.size(), 1);
  QVERIFY(errors.contains("modelName"));
  QVERIFY(!errors.contains("brandName"));

  // Только модель заполнена
  car->setBrandName("");
  car->setModelName("Leon");
  errors = car->validateAll();
  QCOMPARE(errors.size(), 1);
  QVERIFY(errors.contains("brandName"));

  // Оба поля заполнены (SEAT)
  car->setBrandName("SEAT");
  car->setModelName("Leon");
  errors = car->validateAll();
  QCOMPARE(errors.size(), 0);

  // Оба поля заполнены (Cupra)
  car->setBrandName("Cupra");
  car->setModelName("Formentor");
  errors = car->validateAll();
  QCOMPARE(errors.size(), 0);
}

//=============================================================================
// Тесты вычисляемых свойств
//=============================================================================

void TestCarInfo::testName()
{
  // Невалидный автомобиль
  QCOMPARE(car->name(), QString());

  // Только бренд
  car->setBrandName("SEAT");
  QCOMPARE(car->name(), QString());  // Все еще невалидно

  // Бренд и модель
  car->setModelName("Leon");
  QCOMPARE(car->name(), "SEAT Leon");

  // Смена бренда на Cupra
  car->setBrandName("Cupra");
  QCOMPARE(car->name(), "Cupra Leon");

  // Смена модели
  car->setModelName("Ateca");
  QCOMPARE(car->name(), "Cupra Ateca");

  // Очистка модели
  car->setModelName("");
  QCOMPARE(car->name(), QString());

  // Проверка других комбинаций
  car->setBrandName("SEAT");
  car->setModelName("Arona");
  QCOMPARE(car->name(), "SEAT Arona");

  car->setBrandName("Cupra");
  car->setModelName("Born");
  QCOMPARE(car->name(), "Cupra Born");
}

void TestCarInfo::testIsValid()
{
  // Пустой автомобиль
  QVERIFY(!car->isValid());

  // Только бренд SEAT
  car->setBrandName("SEAT");
  QVERIFY(!car->isValid());

  // Только бренд Cupra
  car->setBrandName("");
  car->setBrandName("Cupra");
  QVERIFY(!car->isValid());

  // Только модель
  car->setBrandName("");
  car->setModelName("Leon");
  QVERIFY(!car->isValid());

  // Бренд и модель (SEAT)
  car->setBrandName("SEAT");
  car->setModelName("Leon");
  QVERIFY(car->isValid());

  // Бренд и модель (Cupra)
  car->setBrandName("Cupra");
  car->setModelName("Formentor");
  QVERIFY(car->isValid());

  // Проверяем, что isValid зависит только от brand и model
  car->setMileage(1000);  // mileage не влияет на isValid
  QVERIFY(car->isValid());

  car->setMileageUpdateDate(QDate(2020, 1, 1));  // дата не влияет
  QVERIFY(car->isValid());
}

//=============================================================================
// Тесты сложных сценариев
//=============================================================================

void TestCarInfo::testMultipleUpdates()
{
  QSignalSpy brandSpy(car, &CarInfo::brandNameChanged);
  QSignalSpy modelSpy(car, &CarInfo::modelNameChanged);
  QSignalSpy nameSpy(car, &CarInfo::nameChanged);
  QSignalSpy isValidSpy(car, &CarInfo::isValidChanged);

  // Последовательные обновления
  car->setBrandName("SEAT");
  car->setModelName("Altea Freetrack");
  car->setMileage(15000);

  QCOMPARE(brandSpy.count(), 1);
  QCOMPARE(modelSpy.count(), 1);
  QCOMPARE(nameSpy.count(), 2);     // brand и model изменения
  QCOMPARE(isValidSpy.count(), 1);  // стал валидным

  // Финальная проверка
  QCOMPARE(car->brandName(), "SEAT");
  QCOMPARE(car->modelName(), "Altea Freetrack");
  QCOMPARE(car->mileage(), 15000);
  QCOMPARE(car->name(), "SEAT Altea Freetrack");
  QVERIFY(car->isValid());

  // Обновление до Cupra
  car->setBrandName("Cupra");
  car->setModelName("Formentor");

  QCOMPARE(car->brandName(), "Cupra");
  QCOMPARE(car->modelName(), "Formentor");
  QCOMPARE(car->name(), "Cupra Formentor");
  QVERIFY(car->isValid());
}

void TestCarInfo::testIsValidTransitions()
{
  // Тестируем все комбинации isValid
  struct TestCase
  {
    QString brand;
    QString model;
    bool    expectedIsValid;
  };

  QVector<TestCase> testCases = {
    { "", "", false },
    { "SEAT", "", false },
    { "", "Altea Freetrack", false },
    { "SEAT", "Altea Freetrack", true },
    { "   ", "Altea Freetrack", false },  // Строка с пробелами не пустая
    { "SEAT", "   ", false },             // Строка с пробелами не пустая
  };

  for (const auto& tc : testCases)
  {
    car->setBrandName(tc.brand);
    car->setModelName(tc.model);
    QCOMPARE(car->isValid(), tc.expectedIsValid);
  }
}

void TestCarInfo::testNoSignalsOnSameValue()
{
  QSignalSpy brandSpy(car, &CarInfo::brandNameChanged);
  QSignalSpy modelSpy(car, &CarInfo::modelNameChanged);
  QSignalSpy mileageSpy(car, &CarInfo::mileageChanged);
  QSignalSpy dateSpy(car, &CarInfo::mileageUpdateDateChanged);
  QSignalSpy nameSpy(car, &CarInfo::nameChanged);
  QSignalSpy isValidSpy(car, &CarInfo::isValidChanged);

  // Устанавливаем начальные значения
  car->setBrandName("SEAT");
  car->setModelName("Leon");
  car->setMileage(1000);
  car->setMileageUpdateDate(QDate(2026, 3, 20));

  // Сбрасываем счетчики
  brandSpy.clear();
  modelSpy.clear();
  mileageSpy.clear();
  dateSpy.clear();
  nameSpy.clear();
  isValidSpy.clear();

  // Повторная установка тех же значений
  car->setBrandName("SEAT");
  car->setModelName("Leon");
  car->setMileage(1000);
  car->setMileageUpdateDate(QDate(2026, 3, 20));

  QCOMPARE(brandSpy.count(), 0);
  QCOMPARE(modelSpy.count(), 0);
  QCOMPARE(mileageSpy.count(), 0);
  QCOMPARE(dateSpy.count(), 0);
  QCOMPARE(nameSpy.count(), 0);
  QCOMPARE(isValidSpy.count(), 0);
}

//=============================================================================
// Тесты граничных значений
//=============================================================================

void TestCarInfo::testEdgeCases()
{
  // Очень длинные строки
  QString longString(10000, 'A');
  car->setBrandName(longString);
  car->setModelName(longString);
  QCOMPARE(car->brandName(), longString);
  QCOMPARE(car->modelName(), longString);
  QVERIFY(car->isValid());
  QCOMPARE(car->name().length(), longString.length() * 2 + 1);

  // Спецсимволы и юникод
  car->setBrandName("SEAT 2024!@#$%^&*()");
  car->setModelName("Leon (Diesel/Гибрид) αβγ");
  QCOMPARE(car->name(), "SEAT 2024!@#$%^&*() Leon (Diesel/Гибрид) αβγ");

  // Нулевые символы в строках
  QString withNull = "Cupra\0Formentor";
  car->setBrandName(withNull);
  QCOMPARE(car->brandName(), withNull);  // Qt строки могут содержать \0

  // Экстремальные даты
  QDate minDate(1, 1, 1);
  QDate maxDate(9999, 12, 31);

  car->setMileageUpdateDate(minDate);
  QCOMPARE(car->mileageUpdateDate(), minDate);

  car->setMileageUpdateDate(maxDate);
  QCOMPARE(car->mileageUpdateDate(), maxDate);
}

void TestCarInfo::testInvariants()
{
  // Тест с SEAT
  car->setBrandName("SEAT");
  QVERIFY(!car->isValid());
  QVERIFY(car->name().isEmpty());

  car->setModelName("Leon");
  QVERIFY(car->isValid());
  QVERIFY(!car->name().isEmpty());
  QCOMPARE(car->name(), "SEAT Leon");

  // Тест с Cupra
  car->setBrandName("Cupra");
  car->setModelName("Formentor");
  QVERIFY(car->isValid());
  QCOMPARE(car->name(), "Cupra Formentor");

  // Очистка
  car->setBrandName("");
  car->setModelName("");
  QVERIFY(!car->validateAll().isEmpty());
  QVERIFY(!car->isValid());

  // Восстановление
  car->setBrandName("SEAT");
  car->setModelName("Arona");
  QVERIFY(car->validateAll().isEmpty());
  QVERIFY(car->isValid());
  QCOMPARE(car->name(), "SEAT Arona");

  car->setMileageUpdateDate(QDate());
  QVERIFY(!car->mileageUpdateDate().isValid());  // Допустимо по реализации
}