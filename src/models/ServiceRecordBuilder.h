#ifndef SERVICERECORDBUILDER_H
#define SERVICERECORDBUILDER_H

#include <qqmlintegration.h>
#include <QObject>

class ServiceRecord;

class ServiceRecordBuilder : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

 public:
  explicit ServiceRecordBuilder(QObject* parent = nullptr);

  Q_INVOKABLE ServiceRecord* create(const QVariantMap& data, QObject* parent = nullptr);
  Q_INVOKABLE ServiceRecord* createEmpty(QObject* parent = nullptr);
  Q_INVOKABLE ServiceRecord* fromJSON(const QString& jsonString, QObject* parent = nullptr);

 signals:
};

#endif  // SERVICERECORDBUILDER_H
