#include <QDirIterator>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QIcon>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QTranslator>
#include <QQmlContext>

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  QLoggingCategory::setFilterRules(QStringLiteral("qt.translator.debug=true"));

/*
  QDirIterator it(":", QDirIterator::Subdirectories);
  while (it.hasNext()) {
    qDebug() << "Ресурс:" << it.next();
  }
*/

  QTranslator translator;
  if (translator.load(QLocale(QLocale::system().name()), "BibikaService", "_", ":/lang")) {
    qDebug() << "Load translations successfully";
    app.installTranslator(&translator);
  }
  else {
    qDebug() << "Load translations failed";
  }

  QQuickStyle::setStyle("Material");
  app.setOrganizationDomain("ru.blog2k.bibikaservice");
  app.setOrganizationName("Personal");

  QQmlApplicationEngine engine;

  QObject::connect(
    &engine,
    &QQmlApplicationEngine::objectCreationFailed,
    &app,
    []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);
  engine.addImportPath(":/");
  engine.loadFromModule("BibikaService", "Main");

  if (engine.rootObjects().isEmpty()) {
    engine.load(QUrl("qrc:/qml/Main.qml"));
  }

  return app.exec();
}
