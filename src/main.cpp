#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  QQuickStyle::setStyle("FluentWinUI3");
  app.setOrganizationName("Evgenii Zhirnov");
  app.setOrganizationDomain("blog2k.ru");

  QQmlApplicationEngine engine;
  QObject::connect(
    &engine,
    &QQmlApplicationEngine::objectCreationFailed,
    &app,
    []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);
  engine.loadFromModule("BibikaService", "Main");

  return app.exec();
}
