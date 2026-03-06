#include "ServiceRecordBuilder.h"
#include "ServiceRecord.h"


ServiceRecordBuilder::ServiceRecordBuilder(QObject* parent) : QObject{ parent }
{
}

ServiceRecord *ServiceRecordBuilder::fromJSON(const QString &jsonString, QObject *parent)
{
  return ServiceRecord::fromJSON(jsonString, parent);
}
