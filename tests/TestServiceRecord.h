#ifndef TESTSERVICERECORD_H
#define TESTSERVICERECORD_H

#include <QObject>

class TestServiceRecord : public QObject
{
  Q_OBJECT

 private slots:
  void testDefaultConstructor();
  void testSettersAndGetters();
  void testSignals();
  void testJson();
};

#endif  // TESTSERVICERECORD_H