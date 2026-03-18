#ifndef TESTSERVICERECORD_H
#define TESTSERVICERECORD_H

#include <QtTest>
#include <QObject>
#include <QDate>
#include <ServiceRecord.h>

class TestServiceRecord : public QObject
{
  Q_OBJECT

 private slots:
  // Инициализация
  void init();
  void cleanup();

  void testDefaultConstructor();
  void testSettersAndGetters();
  void testSignals();
  void testJson();
};

#endif // TESTSERVICERECORD_H