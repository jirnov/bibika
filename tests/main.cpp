#include <QTest>
#include "TestServiceRecord.h"
#include "TestServiceRecordModel.h"


template <typename... ClassList>
int runTests(int argc, char* argv[])
{
  int status = 0;

  auto runTest = [&](auto testClass) {
    decltype(testClass) obj;
    status |= QTest::qExec(&obj, argc, argv);
  };

  (runTest(ClassList{}), ...);

  return status;
}

int main(int argc, char* argv[])
{
  QCoreApplication app(argc, argv);

  return runTests<TestServiceRecord, TestServiceRecordModel>(argc, argv);
}
