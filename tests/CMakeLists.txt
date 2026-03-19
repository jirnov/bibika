add_executable(
    bibikaService_tester
    main.cpp
    TestServiceRecord.cpp
    TestServiceRecord.h
    TestServiceRecordModel.cpp
    TestServiceRecordModel.h
    TestCarInfo.h
    TestCarInfo.cpp
    TestCarInfoBuilder.h
    TestCarInfoBuilder.cpp
    ${COMMON_SOURCES})

target_include_directories(bibikaService_tester PRIVATE ${INCLUDE_DIRS})
target_include_directories(bibikaService_tester PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bibikaService_tester PRIVATE Qt6::Test)
target_link_libraries(bibikaService_tester PRIVATE ${LIBRARIES})

add_test(NAME TestServiceRecord COMMAND bibikaService_tester -test TestServiceRecord)
add_test(NAME TestServiceRecordModel COMMAND bibikaService_tester -test TestServiceRecordModel)
add_test(NAME TestCarInfo COMMAND bibikaService_tester -test TestCarInfo)
add_test(NAME TestCarInfoBuilder COMMAND bibikaService_tester -test TestCarInfoBuilder)
