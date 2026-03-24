cmake_minimum_required(VERSION 3.16)

project(BibikaService VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS
    Quick
    QuickControls2
    Core
    Test
    Qml
    LinguistTools
    Sql)

# Включаем ресурсы по умолчаию :/qt/qml
qt_policy(SET QTP0001 NEW)

# Выключаем предупреждения о qmldir
qt_policy(SET QTP0004 NEW)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service
)

set(COMMON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/CarInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/CarInfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/ServiceRecord.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/ServiceRecord.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/ServiceRecordBuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/ServiceRecordBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/CarInfoBuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/CarInfoBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service/CarModelIndex.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service/CarModelIndex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/ServiceRecordModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/ServiceRecordModel.cpp)

set(LIBRARIES
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Core
    Qt6::Qml
    Qt6::Sql)

qt_standard_project_setup(
    I18N_TRANSLATED_LANGUAGES en)

set(QML_FILES
    qml/Main.qml
    qml/screens/SettingsScreen.qml
    qml/screens/DashboardScreen.qml
    qml/screens/ServiceRecordScreen.qml
    qml/screens/WelcomeScreen.qml
    qml/style/Style.qml
    qml/components/MileageTextField.qml
    qml/dialogs/UpdateMilestoneDialog.qml
    qml/AppSettings.qml
    qml/components/SmartTextField.qml
    qml/components/AcceptButton.qml
)

qt_add_translations(appBibikaService
    RESOURCE_PREFIX /lang
    TS_FILE_DIR lang
)

qt6_add_executable(appBibikaService
    src/main.cpp
)

qt_add_resources(appBibikaService "data"
    BASE "res"
    FILES
        res/cars.json
        res/qtquickcontrols2.conf)

qt_add_resources(appBibikaService "icons"
    BASE "res"
    FILES
        res/icons/calendar.svg
        res/icons/chart.svg
        res/icons/history.svg
)

qt_add_resources(appBibikaService "lang"
    PREFIX "/lang"
    BASE "lang/"
    FILES
        lang/BibikaService_en.ts)

set_source_files_properties(qml/style/Style.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)
set_source_files_properties(qml/AppSettings.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

if(ANDROID)
    set_property(TARGET appBibikaService APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)
endif()

qt6_add_qml_module(appBibikaService
    URI BibikaService
    VERSION 1.0
    QML_FILES
        ${QML_FILES}
    SOURCES
        ${COMMON_SOURCES}
)

target_include_directories(appBibikaService PRIVATE ${INCLUDE_DIRS})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appBibikaService PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER ru.blog2k.bibikaservice
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appBibikaService PRIVATE ${LIBRARIES})

include(GNUInstallDirs)
install(TARGETS appBibikaService
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
