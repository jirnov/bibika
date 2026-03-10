#ifndef CARINFO_H
#define CARINFO_H

#include <QDate>
#include <QObject>
#include <qqmlintegration.h>

class CarInfo : public QObject
{
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(QString brandName READ brandName WRITE setBrandName NOTIFY brandNameChanged FINAL)
  Q_PROPERTY(QString modelName READ modelName WRITE setModelName NOTIFY modelNameChanged FINAL)
  Q_PROPERTY(int lastMileage READ lastMileage WRITE setLastMileage NOTIFY lastMileageChanged FINAL)
  Q_PROPERTY(QDate lastMileageDate READ lastMileageDate NOTIFY lastMileageDateChanged FINAL)
  Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged FINAL)

 public:
  explicit CarInfo(QObject* parent = nullptr);

  Q_INVOKABLE QString toJSON() const;

  static CarInfo *fromJSON(const QString &jsonString, QObject *parent=nullptr);

  QString brandName() const;
  void setBrandName(const QString &brandName);

  QString modelName() const;
  void setModelName(const QString &modelName);

  int lastMileage() const;
  void setLastMileage(const int lastMileage);

  bool isValid() const;

  QDate lastMileageDate() const;

  Q_INVOKABLE QMap<QString, QString> validateAll() const;
  Q_INVOKABLE QString validateBrand() const;
  Q_INVOKABLE QString validateModel() const;

 signals:
  void brandNameChanged();
  void modelNameChanged();
  void lastMileageChanged();
  void lastMileageDateChanged();

  void isValidChanged();

 private:
  void fromJSONString(const QString &jsonString);

  QString m_brandName;
  QString m_modelName;
  int m_lastMileage{0};
  QDate m_lastMileageDate = QDate::currentDate();
};

#endif  // CARINFO_H
