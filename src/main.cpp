#include <QDirIterator>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QIcon>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QTranslator>
#include <QQmlContext>

#ifdef Q_CC_MSVC
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#endif

static void dumpResources()
{
  QDirIterator it(":", QDirIterator::Subdirectories);
  while (it.hasNext())
  {
    qDebug() << "Ресурс:" << it.next();
  }
}

static QTranslator* createTranslator(QObject* parent)
{
  QTranslator* translator = new QTranslator(parent);
  if (translator->load(QLocale(QLocale::system().name()), "BibikaService", "_", ":/lang"))
  {
    return translator;
  }
  return nullptr;
}

int runApp(int argc, char* argv[])
{
  QGuiApplication app(argc, argv);

  if (auto translator = createTranslator(&app))
  {
    app.installTranslator(translator);
    qDebug() << "=== Translation loaded successfully ===";
  }
  else
  {
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

  if (engine.rootObjects().isEmpty())
  {
    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
  }

  QObject::connect(&engine, &QQmlEngine::quit, &app, &QGuiApplication::quit);

  return app.exec();
}

int main(int argc, char* argv[])
{
#ifdef Q_CC_MSVC
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
  //_CrtSetBreakAlloc(523549);
#endif

  // Отключаем вывод предупреждений доступности
  QLoggingCategory::setFilterRules("qt.a11y.*=false");
  QLoggingCategory::setFilterRules("qt.core.translator=true");
  QLoggingCategory::setFilterRules("qt.accessibility.*=false");

  qputenv("QT_ANDROID_DISABLE_ACCESSIBILITY", "1");

  return runApp(argc, argv);
}
