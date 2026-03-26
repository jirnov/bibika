#include "NotificationHelper.h"
#include <QCoreApplication>
#include <QJniEnvironment>

void NotificationHelper::showNotification(const QString& title, const QString& message)
{
    QJniObject javaTitle = QJniObject::fromString(title);
    QJniObject javaMessage = QJniObject::fromString(message);

    QJniObject::callStaticMethod<void>("org/qtproject/bibika/NotificationHelper",
                                       "show",
                                       "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;)V",
                                       QNativeInterface::QAndroidApplication::context(),
                                       javaTitle.object<jstring>(),
                                       javaMessage.object<jstring>());
}