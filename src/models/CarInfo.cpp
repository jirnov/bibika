#include "CarInfo.h"

#include <QJsonDocument>
#include <QJsonObject>


CarInfo::CarInfo(QObject* parent) : QObject{ parent }
{
  connect(this, &CarInfo::brandNameChanged, this, &CarInfo::isValidChanged);
  connect(this, &CarInfo::modelNameChanged, this, &CarInfo::isValidChanged);
  connect(this, &CarInfo::lastMileageChanged, this, &CarInfo::lastMileageDateChanged);
}

QString CarInfo::toJSON() const
{
  QJsonObject json {
    {"brandName", m_brandName},
    {"modelName", m_modelName},
    {"lastMileage", m_lastMileage},
    {"lastMileageDate", m_lastMileageDate.toString(Qt::ISODate)}
  };
  return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

CarInfo *CarInfo::fromJSON(const QString &jsonString, QObject *parent)
{
  CarInfo *carInfo = new CarInfo(parent);
  carInfo->fromJSONString(jsonString);
  return carInfo;
}

void CarInfo::fromJSONString(const QString &jsonString)
{
  QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
  if (!doc.isNull() && doc.isObject()) {
    QJsonObject json = doc.object();
    setBrandName(json["brandName"].toString());
    setModelName(json["modelName"].toString());
    setLastMileage(json["lastMileage"].toInt());

    const auto date = QDate::fromString(json["lastMileageDate"].toString(), Qt::ISODate);
    if (date.isValid()) {
      m_lastMileageDate = date;
    }
    else {
      m_lastMileageDate = QDate::currentDate();
    }
    emit lastMileageDateChanged();
  }
}

QString CarInfo::brandName() const {
  return m_brandName;
}

void CarInfo::setBrandName(const QString &newBrandName)
{
  if (m_brandName == newBrandName) {
    return;
  }

  m_brandName = newBrandName;
  emit brandNameChanged();
}

QString CarInfo::modelName() const
{
  return m_modelName;
}

void CarInfo::setModelName(const QString &newModelName)
{
  if (m_modelName == newModelName) {
    return;
  }
  m_modelName = newModelName;
  emit modelNameChanged();
}

int CarInfo::lastMileage() const
{
  return m_lastMileage;
}

void CarInfo::setLastMileage(const int newLastMileage)
{
  if (m_lastMileage == newLastMileage) {
    return;
  }
  m_lastMileage = newLastMileage;
  m_lastMileageDate = QDate::currentDate();
  emit lastMileageChanged();
}

bool CarInfo::isValid() const
{
  return validateAll().empty();
}

QDate CarInfo::lastMileageDate() const
{
  return m_lastMileageDate;
}

QMap<QString, QString> CarInfo::validateAll() const {
  QMap<QString, QString> errors;

  const auto brandError = validateBrand();
  if (!brandError.isEmpty()) {
    errors["brandName"] = brandError;
  }

  const auto modelError = validateModel();
  if (modelError.isEmpty()) {
    errors["modelName"] = modelError;
  }

  return errors;
}

QString CarInfo::validateBrand() const
{
  if (m_brandName.isEmpty()) {
    return tr("Пожалуйста, укажите марку (обязательное поле)");
  }
  return {};
}

QString CarInfo::validateModel() const
{
  if (m_modelName.isEmpty()) {
    return tr("Пожалуйста, укажите модель (обязательное поле)");
  }
  return {};
}


