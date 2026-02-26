cmake_minimum_required(VERSION 3.16)

project(BibikaService VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

set(QML_FILES
    qml/Main.qml
    qml/screens/WelcomeScreen.qml
    qml/screens/DashboardScreen.qml
    qml/screens/SettingsScreen.qml
    qml/screens/EditServiceRecordScreen.qml
    qml/dialogs/UpdateMilestoneDialog.qml
    qml/components/MileageTextField.qml
)

qt_add_resources(RESOURCES res/res.qrc)

qt_add_executable(appBibikaService
    src/main.cpp
    ${RESOURCES}
)

qt_add_qml_module(appBibikaService
    URI BibikaService
    QML_FILES
        ${QML_FILES}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appBibikaService PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appBibikaService
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appBibikaService
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appBibikaService
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
