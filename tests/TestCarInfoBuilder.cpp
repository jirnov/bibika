#include "TestCarInfoBuilder.h"
#include "CarInfoBuilder.h"
#include "CarInfo.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QScopedPointer>

void TestCarInfoBuilder::initTestCase()
{
  builder = new CarInfoBuilder(this);

  validJsonFull = R"({
        "brandName": "Toyota",
        "modelName": "Camry",
        "mileage": 15000,
        "mileageUpdateDate": "2026-03-19"
    })";

  validJsonPartial = R"({
        "brandName": "Honda",
        "mileage": 5000
    })";
}

//=============================================================================
// Тесты createEmpty
//=============================================================================

void TestCarInfoBuilder::testCreateEmpty()
{
  CarInfo* car = builder->createEmpty(this);
  QVERIFY(car != nullptr);

  // Проверяем значения по умолчанию
  QCOMPARE(car->brandName(), QString());
  QCOMPARE(car->modelName(), QString());
  QCOMPARE(car->mileage(), 0);
  QVERIFY(!car->mileageUpdateDate().isValid());
  QCOMPARE(car->mileageUpdateDate(), QDate());

  delete car;  // Важно: удаляем, т.к. parent не указан
}

void TestCarInfoBuilder::testCreateEmptyWithParent()
{
  QObject  parent;
  CarInfo* car = builder->createEmpty(&parent);

  QVERIFY(car != nullptr);
  QCOMPARE(car->parent(), &parent);

  // Проверяем, что объект будет удален вместе с parent
  // (просто проверяем parent, т.к. удаление проверить сложно)

  // Не удаляем car вручную, parent удалит его автоматически
}

//=============================================================================
// Тесты fromJSON
//=============================================================================

void TestCarInfoBuilder::testFromJSON_ValidFull()
{
  CarInfo* car = builder->fromJSON(validJsonFull, this);
  QVERIFY(car != nullptr);
  QScopedPointer<CarInfo> cleanup(car);  // Автоудаление

  QCOMPARE(car->brandName(), QString("Toyota"));
  QCOMPARE(car->modelName(), QString("Camry"));
  QCOMPARE(car->mileage(), 15000);
  QCOMPARE(car->mileageUpdateDate(), QDate(2026, 3, 19));
  QVERIFY(car->isValid());
}

void TestCarInfoBuilder::testFromJSON_ValidPartial()
{
  CarInfo* car = builder->fromJSON(validJsonPartial, this);
  QVERIFY(car != nullptr);
  QScopedPointer<CarInfo> cleanup(car);

  QCOMPARE(car->brandName(), QString("Honda"));
  QCOMPARE(car->modelName(), QString());  // Не указано в JSON
  QCOMPARE(car->mileage(), 5000);

  // Дата должна быть текущей (не указана в JSON)
  QCOMPARE(car->mileageUpdateDate(), QDate());

  QVERIFY(!car->isValid());  // Модель не заполнена
}

void TestCarInfoBuilder::testFromJSON_InvalidJson()
{
  QString invalidJson = R"({ "brandName": "Toyota", missing closing )";

  CarInfo* car = builder->fromJSON(invalidJson, this);
  QVERIFY(car == nullptr);  // Должен вернуть nullptr при ошибке
}

void TestCarInfoBuilder::testFromJSON_EmptyJson()
{
  QString emptyJson = "{}";

  CarInfo* car = builder->fromJSON(emptyJson, this);
  QVERIFY(car != nullptr);
  QScopedPointer<CarInfo> cleanup(car);

  // Проверяем значения по умолчанию
  QCOMPARE(car->brandName(), QString());
  QCOMPARE(car->modelName(), QString());
  QCOMPARE(car->mileage(), 0);
  QCOMPARE(car->mileageUpdateDate(), QDate{});
}

void TestCarInfoBuilder::testFromJSON_ExtraFields()
{
  QString jsonWithExtra = R"({
        "brandName": "Toyota",
        "modelName": "Camry",
        "mileage": 15000,
        "extraField": "should be ignored",
        "anotherExtra": 123,
        "color": "red"
    })";

  CarInfo* car = builder->fromJSON(jsonWithExtra, this);
  QVERIFY(car != nullptr);
  QScopedPointer<CarInfo> cleanup(car);

  // Проверяем, что основные поля загрузились правильно
  QCOMPARE(car->brandName(), QString("Toyota"));
  QCOMPARE(car->modelName(), QString("Camry"));
  QCOMPARE(car->mileage(), 15000);
}

void TestCarInfoBuilder::testFromJSON_InvalidTypes()
{
  // mileage передано как строка вместо числа
  QString jsonInvalidMileage = R"({
        "brandName": "Toyota",
        "modelName": "Camry",
        "mileage": "15000"
    })";

  CarInfo* car = builder->fromJSON(jsonInvalidMileage, this);
  QVERIFY(car != nullptr);  // Должен создать, игнорируя неверное поле
  QScopedPointer<CarInfo> cleanup(car);

  QCOMPARE(car->brandName(), QString("Toyota"));
  QCOMPARE(car->modelName(), QString("Camry"));
  QCOMPARE(car->mileage(), 0);  // Должен быть 0 (значение по умолчанию)

  // Неверный формат даты
  QString jsonInvalidDate = R"({
        "mileageUpdateDate": "not-a-date"
    })";

  car = builder->fromJSON(jsonInvalidDate, this);
  QVERIFY(car != nullptr);
  cleanup.reset(car);

  QCOMPARE(car->mileageUpdateDate(), QDate());
}

void TestCarInfoBuilder::testFromJSON_WithParent()
{
  QObject  parent;
  CarInfo* car = builder->fromJSON(validJsonFull, &parent);

  QVERIFY(car != nullptr);
  QCOMPARE(car->parent(), &parent);
  QCOMPARE(car->brandName(), QString("Toyota"));
}

//=============================================================================
// Тесты toJSON
//=============================================================================

void TestCarInfoBuilder::testToJSON_CompleteCar()
{
  CarInfo car;
  car.setBrandName("Toyota");
  car.setModelName("Camry");
  car.setMileage(15000);
  car.setMileageUpdateDate(QDate(2026, 3, 19));

  QString jsonString = builder->toJSON(&car);
  QVERIFY(!jsonString.isEmpty());

  // Проверяем, что это валидный JSON
  QJsonParseError error;
  QJsonDocument   doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
  QCOMPARE(error.error, QJsonParseError::NoError);
  QVERIFY(doc.isObject());

  QJsonObject obj = doc.object();
  QCOMPARE(obj["brandName"].toString(), QString("Toyota"));
  QCOMPARE(obj["modelName"].toString(), QString("Camry"));
  QCOMPARE(obj["mileage"].toInt(), 15000);
  QCOMPARE(obj["mileageUpdateDate"].toString(), QString("2026-03-19"));
}

void TestCarInfoBuilder::testToJSON_EmptyCar()
{
  CarInfo car;  // Все поля по умолчанию
  car.setMileageUpdateDate(QDate::currentDate());

  QString jsonString = builder->toJSON(&car);
  QVERIFY(!jsonString.isEmpty());

  QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
  QVERIFY(doc.isObject());

  QJsonObject obj = doc.object();

  // Пустые строки должны быть в JSON
  QCOMPARE(obj["brandName"].toString(), QString());
  QCOMPARE(obj["modelName"].toString(), QString());
  QCOMPARE(obj["mileage"].toInt(), 0);

  // Дата должна быть сегодня
  QCOMPARE(obj["mileageUpdateDate"].toString(), QDate::currentDate().toString(Qt::ISODate));
}

void TestCarInfoBuilder::testToJSON_PartialCar()
{
  CarInfo car;
  car.setBrandName("Honda");
  car.setMileage(5000);
  // modelName не установлен

  QString       jsonString = builder->toJSON(&car);
  QJsonDocument doc        = QJsonDocument::fromJson(jsonString.toUtf8());
  QJsonObject   obj        = doc.object();

  QCOMPARE(obj["brandName"].toString(), QString("Honda"));
  QCOMPARE(obj["mileage"].toInt(), 5000);

  // modelName должен быть пустой строкой, а не отсутствовать
  QVERIFY(obj.contains("modelName"));
  QCOMPARE(obj["modelName"].toString(), QString());
}

void TestCarInfoBuilder::testToJSON_SpecialCharacters()
{
  CarInfo car;
  car.setBrandName("BMW 2024!@#$%^&*()");
  car.setModelName("X5 (Diesel/Гибрид)");
  car.setMileage(12345);

  QString       jsonString = builder->toJSON(&car);
  QJsonDocument doc        = QJsonDocument::fromJson(jsonString.toUtf8());
  QVERIFY(doc.isObject());

  QJsonObject obj = doc.object();
  QCOMPARE(obj["brandName"].toString(), QString("BMW 2024!@#$%^&*()"));
  QCOMPARE(obj["modelName"].toString(), QString("X5 (Diesel/Гибрид)"));
}

void TestCarInfoBuilder::testToJSON_ExtremeValues()
{
  CarInfo car;
  car.setBrandName(QString(1000, 'A'));  // Очень длинная строка
  car.setModelName(QString(1000, 'B'));
  car.setMileage(std::numeric_limits<int>::max());
  car.setMileageUpdateDate(QDate(9999, 12, 31));  // Очень далекая дата

  QString jsonString = builder->toJSON(&car);
  QVERIFY(jsonString.length() > 2000);  // Должна быть длинной

  QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
  QVERIFY(doc.isObject());

  QJsonObject obj = doc.object();
  QCOMPARE(obj["brandName"].toString().length(), 1000);
  QCOMPARE(obj["mileage"].toInt(), std::numeric_limits<int>::max());
  QCOMPARE(obj["mileageUpdateDate"].toString(), QString("9999-12-31"));
}

//=============================================================================
// Интеграционные тесты
//=============================================================================

void TestCarInfoBuilder::testSerializationRoundTrip()
{
  // Создаем оригинальный автомобиль
  CarInfo original;
  original.setBrandName("Tesla");
  original.setModelName("Model 3");
  original.setMileage(7500);
  original.setMileageUpdateDate(QDate(2026, 2, 15));

  // Сериализуем
  QString json = builder->toJSON(&original);
  QVERIFY(!json.isEmpty());

  // Десериализуем
  CarInfo* deserialized = builder->fromJSON(json, this);
  QVERIFY(deserialized != nullptr);
  QScopedPointer<CarInfo> cleanup(deserialized);

  // Проверяем совпадение всех полей
  QCOMPARE(deserialized->brandName(), original.brandName());
  QCOMPARE(deserialized->modelName(), original.modelName());
  QCOMPARE(deserialized->mileage(), original.mileage());
  QCOMPARE(deserialized->mileageUpdateDate(), original.mileageUpdateDate());
  QCOMPARE(deserialized->isValid(), original.isValid());
  QCOMPARE(deserialized->name(), original.name());
}

void TestCarInfoBuilder::testMultipleSerializations()
{
  CarInfo car;
  car.setBrandName("Volvo");
  car.setModelName("XC90");

  // Сериализуем несколько раз
  QString json1 = builder->toJSON(&car);
  QString json2 = builder->toJSON(&car);
  QString json3 = builder->toJSON(&car);

  // Все результаты должны быть одинаковыми
  QCOMPARE(json1, json2);
  QCOMPARE(json2, json3);

  // Изменяем car
  car.setMileage(10000);
  QString json4 = builder->toJSON(&car);
  QVERIFY(json4 != json1);  // Должен отличаться
}

void TestCarInfoBuilder::testNullPointer()
{
  // Проверяем поведение с nullptr
  QString json = builder->toJSON(nullptr);
  QVERIFY(!json.isEmpty());

  CarInfo* car = builder->fromJSON(validJsonFull, nullptr);
  QVERIFY(car != nullptr);
  QVERIFY(car->parent() == nullptr);
  delete car;  // Важно удалить

  car = builder->createEmpty(nullptr);
  QVERIFY(car != nullptr);
  QVERIFY(car->parent() == nullptr);
  delete car;
}

//=============================================================================
// Дополнительные тесты для edge cases
//=============================================================================

void TestCarInfoBuilder::testFromJSON_WithTimeInDate()
{
  // Иногда дата может прийти с временем
  QString jsonWithTime = R"({
        "mileageUpdateDate": "2026-03-19T15:30:00"
    })";

  CarInfo* car = builder->fromJSON(jsonWithTime, this);
  QVERIFY(car != nullptr);
  QScopedPointer<CarInfo> cleanup(car);

  // Должна извлечься только дата
  QCOMPARE(car->mileageUpdateDate(), QDate(2026, 3, 19));
}

void TestCarInfoBuilder::testFromJSON_WithInvalidDate()
{
  QString jsonWithInvalidDate = R"({
        "mileageUpdateDate": "2026-13-45"
    })";

  CarInfo* car = builder->fromJSON(jsonWithInvalidDate, this);
  QVERIFY(car != nullptr);
  QScopedPointer<CarInfo> cleanup(car);

  QCOMPARE(car->mileageUpdateDate(), QDate());
}