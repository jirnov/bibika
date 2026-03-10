#include "CarModelIndex.h"
#include <qdebug.h>

#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>


CarModelIndex::CarModelIndex(QObject* parent) : QObject{ parent }
{
}

void CarModelIndex::loadFromFile(const QString &jsonPath)
{
  QFile file(jsonPath);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning() << "Cannot open file for reading: " << jsonPath;
    return;
  }

  QByteArray jsonData = file.readAll();
  file.close();

  if (parseJSON(jsonData)) {
    qDebug() << "Load brands: " << m_brands.size();
    emit brandsChanged();
  }
  else {
    qWarning() << "parseJSON() failed";
  }
}

QStringList CarModelIndex::getModelsForBrand(const QString &brand) const
{
  if (brand.isEmpty()) {
    QStringList allModels;
    for (const auto &models : m_models) {
      for (const auto &model : models) {
        allModels.append(model);
      }
    }
    return allModels;
  }

  // Первичное имя бренда
  const QString lookupName = brand.toLower();

  if (m_searchIndex.contains(lookupName)) {
    const int index = m_searchIndex[lookupName];
    return m_models[index];
  }
  return {};
}

QStringList CarModelIndex::brands() const
{
  return m_brands;
}

bool CarModelIndex::parseJSON(const QByteArray &jsonData)
{
  m_brands.clear();
  m_models.clear();
  m_searchIndex.clear();

  QJsonParseError error;
  QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);

  if (error.error != QJsonParseError::NoError) {
    qWarning() << "JSON parse error: " << error.errorString();
    return false;
  }

  if (!doc.isObject()) {
    qWarning() << "JSON is not an object";
    return false;
  }

  const QJsonObject root = doc.object();
  if (!root.contains("brands") || !root["brands"].isArray()) {
    qWarning() << "JSON not includes brands array";
    return false;
  }

  const QJsonArray brandsArray = root["brands"].toArray();

  for (const auto &brandValue : brandsArray) {
    if (!brandValue.isObject()) {
      continue;
    }

    const QJsonObject brandObj = brandValue.toObject();

    const QString name = brandObj["name"].toString();

    QStringList aliases;

    if (brandObj.contains("aliases") && brandObj["aliases"].isArray()) {
      const auto aliasesArr = brandObj["aliases"].toArray();
      for (const auto &alias : aliasesArr) {
        if (alias.isString()) {
          aliases.append(alias.toString());
        }
      }
    }

    aliases.sort();

    QStringList models;

    if (brandObj.contains("models") && brandObj["models"].isArray()) {
      const auto modelsArr = brandObj["models"].toArray();
      for (const auto &model : modelsArr) {
        if (model.isString()) {
          models.append(model.toString());
        }
      }
    }
    models.sort();

    if (!name.isEmpty()) {
      m_searchIndex[name.toLower()] = m_models.size();
      for (const auto &alias : aliases) {
        m_searchIndex[alias.toLower()] = m_models.size();
      }
      m_brands.append(name);
      m_models.append(models);
    }
  }

  return !m_brands.empty();
}
