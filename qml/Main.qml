pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls

import BibikaService

ApplicationWindow {
    id: root
    width: Style.width
    height: Style.height
    minimumWidth: width
    //    maximumWidth: width
    minimumHeight: height
    //    maximumHeight: height
    visible: true
    title: "Прототип приложения \"Бибика\" - обслуживание своего автомобиля"
    modality: Qt.ApplicationModal

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: {
            let c1 = CarInfoBuilder.fromJSON('{"brandName":"SEAT","lastMileage":232000,"lastMileageDate":"2026-03-06","modelName":"Altea Freetrack"}')
            let c2 = CarInfoBuilder.fromJSON('{"brandName":"BMW","lastMileage":0,"modelName":"X5"}')

            console.log(c1.toJSON())
            console.log(c2.toJSON())


            return AppSettings.carInfo.isValid ? dashboardScreen : welcomeScreen;
        }
    }

    Component {
        id: welcomeScreen
        WelcomeScreen {
            carInfo: AppSettings.carInfo

            onAccepted: function(newCarInfo) {
                AppSettings.updateCarInfo(newCarInfo)
                stackView.push(dashboardScreen)
            }
        }
    }

    Component {
        id: dashboardScreen
        DashboardScreen {
            onOpenAddRecordDialog: {
                console.log("open new record dialog")
                stackView.push(serviceRecordScreen, {"currentMileage": AppSettings.carInfo.lastMileage})
            }

            onOpenSettingsDialog: console.log("open settings")
            onOpenMileageDialog: console.log("open mileage dialog")

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
        SettingsScreen {}
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
