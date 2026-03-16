#include "CarInfoBuilder.h"
#include "CarInfo.h"

CarInfoBuilder::CarInfoBuilder(QObject* parent) : QObject{ parent }
{
}

CarInfo *CarInfoBuilder::createEmpty(QObject *parent)
{
  return new CarInfo(parent);
}

CarInfo *CarInfoBuilder::fromJSON(const QString &jsonString, QObject *parent)
{
  return CarInfo::fromJSON(jsonString, parent);
}
