#ifndef SERVICERECORDBUILDER_H
#define SERVICERECORDBUILDER_H

#include <qqmlintegration.h>
#include <QObject>
#include <ServiceRecord.h>

class ServiceRecordBuilder : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

 public:
  explicit ServiceRecordBuilder(QObject* parent = nullptr);

  static Q_INVOKABLE ServiceRecord* createEmpty(QObject* parent = nullptr);
  static Q_INVOKABLE ServiceRecord* fromJSON(const QString& jsonString, QObject* parent = nullptr);
  static Q_INVOKABLE QString        toJSON(ServiceRecord* sr);

  static QString                  eventType2Str(ServiceRecord::EventType eventType);
  static ServiceRecord::EventType str2EventType(const QString& eventTypeStr);
};

#endif  // SERVICERECORDBUILDER_H
