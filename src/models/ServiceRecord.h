#ifndef SERVICERECORD_H
#define SERVICERECORD_H

#include <qqmlintegration.h>
#include <QObject>
#include <QDate>
#include <QQmlEngine>

class ServiceRecord : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(EventType eventType READ eventType WRITE setEventType NOTIFY eventTypeChanged FINAL)
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
  Q_PROPERTY(QString notes READ notes WRITE setNotes NOTIFY notesChanged FINAL)
  Q_PROPERTY(int price READ price WRITE setPrice NOTIFY priceChanged FINAL)
  Q_PROPERTY(int mileage READ mileage WRITE setMileage NOTIFY mileageChanged FINAL)
  Q_PROPERTY(QDate serviceDate READ serviceDate WRITE setServiceDate NOTIFY serviceDateChanged FINAL)

  Q_PROPERTY(int repeatAfterDistance READ repeatAfterDistance WRITE setRepeatAfterDistance NOTIFY
               repeatAfterDistanceChanged FINAL)
  Q_PROPERTY(bool hasRepeatAfterDistance READ hasRepeatAfterDistance WRITE setHasRepeatAfterDistance NOTIFY
               hasRepeatAfterDistanceChanged FINAL)
  Q_PROPERTY(
    int repeatAfterMonths READ repeatAfterMonths WRITE setRepeatAfterMonths NOTIFY repeatAfterMonthsChanged FINAL)
  Q_PROPERTY(bool hasRepeatAfterMonths READ hasRepeatAfterMonths WRITE setHasRepeatAfterMonths NOTIFY
               hasRepeatAfterMonthsChanged FINAL)

  Q_PROPERTY(bool isNameValid READ isNameValid NOTIFY isNameValidChanged FINAL)
  Q_PROPERTY(bool isRepeatDistanceValid READ isRepeatDistanceValid NOTIFY isRepeatDistanceValidChanged FINAL)
  Q_PROPERTY(bool isRepeatMonthsValid READ isRepeatMonthsValid NOTIFY isRepeatMonthsValidChanged FINAL)
  Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged FINAL)

 public:
  explicit ServiceRecord(QObject* parent = nullptr);

  enum EventType
  {
    Invalid = -1,
    Repair,
    Maintenance,
    Service
  };
  Q_ENUM(EventType)

  QString name() const;
  void    setName(const QString& newName);

  QString notes() const;
  void    setNotes(const QString& newNotes);

  int  price() const;
  void setPrice(int newPrice);

  int  mileage() const;
  void setMileage(int newMileage);

  QDate serviceDate() const;
  void  setServiceDate(const QDate& newServiceDate);

  int  repeatAfterDistance() const;
  void setRepeatAfterDistance(int newRepeatAfterDistance);

  bool hasRepeatAfterDistance() const;
  void setHasRepeatAfterDistance(bool newHasRepeatAfterDistance);

  int  repeatAfterMonths() const;
  void setRepeatAfterMonths(int newRepeatAfterMonths);

  bool hasRepeatAfterMonths() const;
  void setHasRepeatAfterMonths(bool newHasRepeatAfterMonths);

  bool isNameValid() const;

  bool isValid() const;

  EventType eventType() const;
  void      setEventType(const EventType& newEventType);

  bool isRepeatDistanceValid() const;

  bool isRepeatMonthsValid() const;

 signals:
  void nameChanged();
  void notesChanged();
  void priceChanged();
  void mileageChanged();
  void serviceDateChanged();
  void repeatAfterDistanceChanged();
  void hasRepeatAfterDistanceChanged();
  void repeatAfterMonthsChanged();
  void hasRepeatAfterMonthsChanged();

  void isNameValidChanged();

  void isValidChanged();

  void eventTypeChanged();

  void isRepeatDistanceValidChanged();

  void isRepeatMonthsValidChanged();

 private:
  QString m_name;
  QString m_notes;
  int     m_price{ 0 };
  int     m_mileage{ 0 };
  QDate   m_serviceDate{ QDate::currentDate() };

  int       m_repeatAfterDistance{ 0 };
  bool      m_hasRepeatAfterDistance{ false };
  int       m_repeatAfterMonths{ 0 };
  bool      m_hasRepeatAfterMonths{ false };
  EventType m_eventType{ EventType::Maintenance };
};

#endif  // SERVICERECORD_H
