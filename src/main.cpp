#include <QDirIterator>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QIcon>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSqlError>
#include <QStandardPaths>
#include <QTranslator>
#include <ServiceRecordModel.h>
#include <ServiceRecordProxy.h>
#include <ServiceRecordRepository.h>

static QSqlDatabase openDatabase()
{
    const QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
                           + "/bibika_service.sqlite";
    const QString connectionName = "service_records_connection";

    QSqlDatabase db;

    if (QSqlDatabase::contains(connectionName))
    {
        db = QSqlDatabase::database(connectionName);
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        if (!db.isValid())
        {
            qWarning() << "Cannot add database" << db.lastError().text();
        }
        db.setDatabaseName(dbPath);
    }

    if (!db.open())
    {
        qWarning() << "Cannot open database" << db.lastError().text();
    }

    return db;
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

    app.setOrganizationDomain("ru.blog2k.bibikaservice");
    app.setOrganizationName("Personal");

    auto repo = std::make_unique<ServiceRecordRepository>(openDatabase(), nullptr);
    ServiceRecordModel serviceRecordModel(std::move(repo), nullptr);
    ServiceRecordProxy serviceRecordProxy(&serviceRecordModel);

    QQmlApplicationEngine engine;

    QObject::connect(
       &engine,
       &QQmlApplicationEngine::objectCreationFailed,
       &app,
       []() {
           QCoreApplication::exit(-1);
       },
       Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("serviceRecordModel", &serviceRecordModel);
    engine.rootContext()->setContextProperty("serviceRecordProxy", &serviceRecordProxy);

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
    // Отключаем вывод предупреждений доступности
    QLoggingCategory::setFilterRules("qt.a11y.*=false");
    QLoggingCategory::setFilterRules("qt.core.translator=true");
    QLoggingCategory::setFilterRules("qt.accessibility.*=false");

    qputenv("QT_ANDROID_DISABLE_ACCESSIBILITY", "1");

    return runApp(argc, argv);
}
