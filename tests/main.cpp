#include "TestCarInfo.h"
#include "TestCarInfoBuilder.h"
#include "TestServiceRecord.h"
#include "TestServiceRecordModel.h"
#include <QTest>

template<typename T>
QString className()
{
    return QString::fromLatin1(T::staticMetaObject.className());
}

template<typename... ClassList>
int runTests(int argc, char* argv[], const QString& selectedTest = {})
{
    static_assert((std::is_base_of_v<QObject, ClassList> && ...), "All test classes must inherit QObject");
    static_assert((std::is_default_constructible_v<ClassList> && ...),
                  "All test classes must be default constructible");

    auto runTest = [&](auto obj) {
        using ObjClass = decltype(obj);
        if (selectedTest.isEmpty() || className<ObjClass>() == selectedTest)
        {
            return QTest::qExec(&obj, argc, argv);
        }
        return 0;
    };

    return (runTest(ClassList{}) | ...);
}

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QString selectedTest;
    QVector<char*> args;
    args.reserve(argc);

    args.push_back(argv[0]);

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-test") == 0 && i + 1 < argc)
        {
            selectedTest = argv[++i];
        }
        else
        {
            args.push_back(argv[i]);
        }
    }

#define TESTS_CLASSES TestServiceRecord, TestServiceRecordModel, TestCarInfo, TestCarInfoBuilder

    int res = runTests<TESTS_CLASSES>(args.size(), args.data(), selectedTest);

#undef TESTS_CLASSES

    return res;
}
