pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls

import BibikaService

ApplicationWindow {
    id: root
    width: Style.width
    height: Style.height
    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height
    visible: true
    title: "Прототип приложения \"Бибика\" - обслуживание своего автомобиля"
    modality: Qt.ApplicationModal

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: AppSettings.showWelcomeScreen ? welcomeScreen : dashboardScreen;
    }

    Component {
        id: welcomeScreen
        WelcomeScreen {
            carInfo: AppSettings.carInfo

            onAccepted: function(newCarInfo) {
                AppSettings.updateCarInfo(newCarInfo)
                AppSettings.showWelcomeScreen = false
                stackView.replace(dashboardScreen)
            }
        }
    }

    UpdateMilestoneDialog {
        id: updateMilestoneDialog
    }

    Component {
        id: dashboardScreen
        DashboardScreen {
            onOpenAddRecordDialog: {
                console.log("open new record dialog")
                stackView.push(serviceRecordScreen, {"currentMileage": AppSettings.carInfo.lastMileage})
            }

            onOpenSettingsDialog: {

                console.log("open settings")
                stackView.push(settingsScreen)
            }
            onOpenMileageDialog: {

                console.log("open mileage dialog")
                updateMilestoneDialog.open()
            }

            onOpenEditRecordDialog: function(index) {
                console.log("open edit dialog for item " + index)

                let editRecord = ServiceRecordBuilder.fromJSON("");
                editRecord.name = "Номер записи: " + index

                stackView.push(serviceRecordScreen, {
                                   "currentMileage": AppSettings.carInfo.lastMileage,
                                   "editRecord": editRecord })
            }
        }
    }

    Component {
        id: settingsScreen
        SettingsScreen {
            onClicked: stackView.pop()
        }
    }

    Component {
        id: serviceRecordScreen
        ServiceRecordScreen{
            onAccepted: function(serviceRecord) {
                console.log("Запись: " + serviceRecord.toJSON())
                stackView.pop()
            }

            onRejected: {
                stackView.pop()
            }
        }
    }
}
