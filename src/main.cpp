#include <QGuiApplication>
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

namespace {

QSqlDatabase openDatabase()
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

void loadTranslations(QCoreApplication* app)
{
    const auto systemLocale = QLocale::system();

    if (systemLocale.language() == QLocale::English)
    {
        QTranslator* translator = new QTranslator(app);
        if (translator->load(":/lang/BibikaService_en.qm"))
        {
            qDebug() << "Load english tranlations successfully";
            app->installTranslator(translator);
        }
        return;
    }
    qDebug() << "Русский язык по умолчанию";
}

int runApp(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    loadTranslations(&app);

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
} // namespace

int main(int argc, char* argv[])
{
    // Отключаем вывод предупреждений доступности
    QLoggingCategory::setFilterRules("qt.a11y.*=false");
    QLoggingCategory::setFilterRules("qt.core.translator=true");
    QLoggingCategory::setFilterRules("qt.accessibility.*=false");

    qputenv("QT_ANDROID_DISABLE_ACCESSIBILITY", "1");

    return runApp(argc, argv);
}
