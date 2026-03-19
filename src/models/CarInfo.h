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
  Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL)
  Q_PROPERTY(int mileage READ mileage WRITE setMileage NOTIFY mileageChanged FINAL)
  Q_PROPERTY(
    QDate mileageUpdateDate READ mileageUpdateDate WRITE setMileageUpdateDate NOTIFY mileageUpdateDateChanged FINAL)
  Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged FINAL)

 public:
  explicit CarInfo(QObject* parent = nullptr);

  QString brandName() const;
  void    setBrandName(const QString& brandName);

  QString modelName() const;
  void    setModelName(const QString& modelName);

  int  mileage() const;
  void setMileage(const int mileage);

  bool isValid() const;

  QDate mileageUpdateDate() const;
  void  setMileageUpdateDate(const QDate& newDate);

  Q_INVOKABLE QMap<QString, QString> validateAll() const;
  Q_INVOKABLE QString                validateBrand() const;
  Q_INVOKABLE QString                validateModel() const;

  QString name() const;

 signals:
  void brandNameChanged();
  void modelNameChanged();
  void mileageChanged();
  void mileageUpdateDateChanged();

  void isValidChanged();

  void nameChanged();

 private:
  QString m_brandName;
  QString m_modelName;
  int     m_mileage{ 0 };
  QDate   m_mileageUpdateDate;
};

#endif  // CARINFO_H
