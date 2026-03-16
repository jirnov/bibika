#ifndef CARMODELINDEX_H
#define CARMODELINDEX_H

#include <QHash>
#include <QMap>
#include <QObject>
#include <qqmlintegration.h>

class CarModelIndex : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(QStringList brands READ brands NOTIFY brandsChanged FINAL)

 public:
  explicit CarModelIndex(QObject* parent = nullptr);

  Q_INVOKABLE void loadFromFile(const QString& jsonPath);

  Q_INVOKABLE QStringList getModelsForBrand(const QString& brand) const;

  QStringList brands() const;

 signals:
  void brandsChanged();

 private:
  bool parseJSON(const QByteArray& jsonData);

  QStringList          m_brands;  // все бренды включая алиасы
  QVector<QStringList> m_models;  // все модели, сгруппированные по брендам
  QHash<QString, int> m_searchIndex;  // для каждого имени бренда ссылка на список моделей
};

#endif  // CARMODELINDEX_H
