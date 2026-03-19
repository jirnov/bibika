#include "CarInfoBuilder.h"
#include "CarInfo.h"

#include <QJsonDocument>
#include <QJsonObject>

CarInfoBuilder::CarInfoBuilder(QObject* parent) : QObject{ parent }
{
}

CarInfo* CarInfoBuilder::createEmpty(QObject* parent)
{
  return new CarInfo(parent);
}

CarInfo* CarInfoBuilder::fromJSON(const QString& jsonString, QObject* parent)
{
  if (jsonString.isEmpty())
  {
    return new CarInfo(parent);
  }

  QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
  if (doc.isNull() || !doc.isObject())
  {
    qWarning() << "Invalid JSON:" << jsonString;
    return nullptr;
  }

  QJsonObject json = doc.object();

  CarInfo* ci = new CarInfo(parent);

  auto setField = [&](auto key, auto converter, auto setter) {
    if (json.contains(key))
    {
      const auto value = converter(json[key]);
      (ci->*setter)(value);
    }
  };

  setField(
    "brandName", [](const auto& v) { return v.toString(); }, &CarInfo::setBrandName);

  setField(
    "modelName", [](const auto& v) { return v.toString(); }, &CarInfo::setModelName);

  setField(
    "mileage", [](const auto& v) { return v.toInt(); }, &CarInfo::setMileage);

  setField(
    "mileageUpdateDate",
    [](const auto& v) {
      auto date = QDate::fromString(v.toString(), Qt::ISODate);
      return date.isValid() ? date : QDate();
    },
    &CarInfo::setMileageUpdateDate);

  return ci;
}

QString CarInfoBuilder::toJSON(CarInfo* ci)
{
  if (!ci)
  {
    return "{}";
  }

  QJsonObject json{ { "brandName", ci->brandName() },
                    { "modelName", ci->modelName() },
                    { "mileage", ci->mileage() },
                    { "mileageUpdateDate", ci->mileageUpdateDate().toString(Qt::ISODate) } };

  return QJsonDocument(json).toJson(QJsonDocument::Compact);
}
