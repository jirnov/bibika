cmake_minimum_required(VERSION 3.16)

project(BibikaService VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Qml)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models
)

qt_standard_project_setup(REQUIRES 6.8)

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

)

qt6_add_executable(appBibikaService
    src/main.cpp
)

set_source_files_properties(qml/style/Style.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)
set_source_files_properties(qml/AppSettings.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt6_add_qml_module(appBibikaService
    URI BibikaService
    VERSION 1.0
    QML_FILES
        ${QML_FILES}
    RESOURCE_PREFIX /
    RESOURCES
        ${QML_FILES}
        res/icons/calendar.svg
        res/icons/chart.svg
        res/icons/history.svg
        QML_FILES
    SOURCES
        src/models/CarInfo.cpp
        src/models/CarInfo.h
        src/models/ServiceRecord.cpp
        src/models/ServiceRecord.h
        src/models/ServiceRecordBuilder.h src/models/ServiceRecordBuilder.cpp
        src/models/CarInfoBuilder.h src/models/CarInfoBuilder.cpp
)

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

target_link_libraries(appBibikaService PRIVATE Qt6::Quick)
target_link_libraries(appBibikaService PRIVATE Qt6::QuickControls2)
target_link_libraries(appBibikaService PRIVATE Qt6::Core)
target_link_libraries(appBibikaService PRIVATE Qt6::Qml)

include(GNUInstallDirs)
install(TARGETS appBibikaService
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
