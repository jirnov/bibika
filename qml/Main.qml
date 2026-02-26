import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCore

import "screens"
import "dialogs"

ApplicationWindow {
    id: mainWindow
    width: 288
    height: 640
    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height
    visible: true
    title: "Прототип приложения \"Бибика\" - обслуживание своего автомобиля"
    modality: Qt.ApplicationModal

    Settings {
        id: settings
        property string brand: "SEAT"
        property string model: "Altea Freetrack"
        property int mileage: 232000
        property date mileageDate: new Date()
        property bool useWelcomeScreen: true
    }

    UpdateMilestoneDialog {
        id: updateMilestoneDialog
    }

    StackView {
        id: rootScreen
        anchors.fill: parent
        initialItem: settings.useWelcomeScreen ? welcomeScreen : dashboardScreen
    }

    Component {
        id: welcomeScreen
        WelcomeScreen {}
    }

    Component {
        id: dashboardScreen
        DashboardScreen {}
    }

    Component {
        id: settingsScreen
        SettingsScreen {}
    }

    Component {
        id: editServiceRecordScreen
        EditServiceRecordScreen {}
    }
}
