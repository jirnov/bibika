#include <QDirIterator>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QIcon>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QTranslator>
#include <QQmlContext>

static void dumpResources()
{
  QDirIterator it(":", QDirIterator::Subdirectories);
  while (it.hasNext()) {
    qDebug() << "Ресурс:" << it.next();
  }
}

static QTranslator *createTranslator(QObject *parent)
{
  QTranslator *translator = new QTranslator(parent);
  if (translator->load(QLocale(QLocale::system().name()), "BibikaService", "_", ":/lang")) {
    return translator;
  }
  return nullptr;
}

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  // Отключаем вывод предупреждений доступности
  QLoggingCategory::setFilterRules("qt.a11y.*=false");

  if (auto translator = createTranslator(&app)) {
    app.installTranslator(translator);
    qDebug() << "=== Translation loaded successfully ===";
  }
  else {
    qWarning() << "!!! Failed to load translation !!!";
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
