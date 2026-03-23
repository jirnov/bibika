#ifndef CARINFOBUILDER_H
#define CARINFOBUILDER_H

#include <qqmlintegration.h>
#include <QObject>

class CarInfo;

class CarInfoBuilder : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

 public:
  explicit CarInfoBuilder(QObject* parent = nullptr);

  Q_INVOKABLE CarInfo* createEmpty(QObject* parent);

  Q_INVOKABLE CarInfo* fromJSON(const QString& jsonString, QObject* parent);
  Q_INVOKABLE QString  toJSON(CarInfo* ci);
};

#endif  // CARINFOBUILDER_H
