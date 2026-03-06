#include <QGuiApplication>
#include <QIcon>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QTranslator>

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  QLoggingCategory::setFilterRules(QStringLiteral("qt.translator.debug=true"));

  QTranslator translator;
  if (translator.load(QLocale(QLocale::system().name()), "BibikaService", "_", ":/lang")) {
    qDebug() << "Load translations successfully";
    app.installTranslator(&translator);
  }
  else {
    qDebug() << "Load translations failed";
  }

  QQuickStyle::setStyle("FluentWinUI3");
  app.setOrganizationDomain("ru.blog2k.bibikaservice");
  app.setOrganizationName("Personal");

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
