#include "CarInfo.h"

#include <QJsonDocument>
#include <QJsonObject>


CarInfo::CarInfo(QObject* parent) : QObject{ parent }
{
  connect(this, &CarInfo::brandNameChanged, this, &CarInfo::nameChanged);
  connect(this, &CarInfo::modelNameChanged, this, &CarInfo::nameChanged);
}


QString CarInfo::brandName() const
{
  return m_brandName;
}

void CarInfo::setBrandName(const QString& newBrandName)
{
  if (m_brandName == newBrandName)
  {
    return;
  }

  const bool oldIsValid = isValid();

  m_brandName = newBrandName;
  emit brandNameChanged();

  if (isValid() != oldIsValid)
  {
    emit isValidChanged();
  }
}

QString CarInfo::modelName() const
{
  return m_modelName;
}

void CarInfo::setModelName(const QString& newModelName)
{
  if (m_modelName == newModelName)
  {
    return;
  }

  const bool oldIsValid = isValid();

  m_modelName = newModelName;
  emit modelNameChanged();
  if (isValid() != oldIsValid)
  {
    emit isValidChanged();
  }
}

int CarInfo::mileage() const
{
  return m_mileage;
}

void CarInfo::setMileage(const int newLastMileage)
{
  if (m_mileage == newLastMileage)
  {
    return;
  }
  m_mileage = newLastMileage;
  emit mileageChanged();
}

bool CarInfo::isValid() const
{
  return validateAll().empty();
}

QDate CarInfo::mileageUpdateDate() const
{
  return m_mileageUpdateDate;
}

void CarInfo::setMileageUpdateDate(const QDate& newDate)
{
  if (m_mileageUpdateDate == newDate)
  {
    return;
  }
  m_mileageUpdateDate = newDate;
  emit mileageUpdateDateChanged();
}

QMap<QString, QString> CarInfo::validateAll() const
{
  QMap<QString, QString> errors;

  const auto brandError = validateBrand();
  if (!brandError.isEmpty())
  {
    errors["brandName"] = brandError;
  }

  const auto modelError = validateModel();
  if (!modelError.isEmpty())
  {
    errors["modelName"] = modelError;
  }

  return errors;
}

QString CarInfo::validateBrand() const
{
  if (m_brandName.trimmed().isEmpty())
  {
    return tr("Пожалуйста, укажите марку (обязательное поле)");
  }
  return {};
}

QString CarInfo::validateModel() const
{
  if (m_modelName.trimmed().isEmpty())
  {
    return tr("Пожалуйста, укажите модель (обязательное поле)");
  }
  return {};
}


QString CarInfo::name() const
{
  if (isValid())
  {
    return m_brandName + " " + m_modelName;
  }
  return QString();
}
