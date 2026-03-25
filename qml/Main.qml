pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls

import BibikaService

ApplicationWindow {
    id: root
    width: AppSettings.width
    height: AppSettings.height
    visible: true
    title: "Прототип приложения \"Бибика\" - обслуживание своего автомобиля"
    modality: Qt.ApplicationModal

    function handleBackButton() {
        if (stackView.depth > 1) {
            stackView.pop();
            return true;
        }
        return false;
    }

    // Для тестирования Back на Windows
    Shortcut {
        sequence: "Esc"

        onActivated: {
            root.handleBackButton();
        }
    }

    onClosing: closeEvent => {
        if (Qt.platform.os === "android") {
            if (root.handleBackButton()) {
                closeEvent.accepted = false;
            } else {
                // TODO: Заставить ещё раз нажать Back для выхода, как у некоторых приложений.
                closeEvent.accepted = true;
            }
        }
    }

    Connections {
        target: Qt.inputMethod
        function onKeyboardRectangleChanged() {
            var height = Qt.inputMethod.keyboardRectangle.height / Screen.devicePixelRatio; // qmllint disable missing-property
            AppSettings.setKeyboardHeight(height);
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: AppSettings.showWelcomeScreen ? welcomeScreen : dashboardScreen
    }

    Component {
        id: welcomeScreen
        WelcomeScreen {
            carInfo: AppSettings.carInfo

            onAccepted: function (newCarInfo) {
                AppSettings.updateCarInfo(newCarInfo);
                AppSettings.showWelcomeScreen = false;
                stackView.replace(dashboardScreen);
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
                console.log("open new record dialog");
                stackView.push(serviceRecordScreen);
            }

            onOpenSettingsDialog: {
                console.log("open settings");
                stackView.push(settingsScreen);
            }
            onOpenMileageDialog: {
                console.log("open mileage dialog");
                updateMilestoneDialog.open();
            }

            onOpenEditRecordDialog: function (recordId) {
                stackView.push(serviceRecordScreen.createObject(stackView, {
                    "recordId": recordId
                }));
            }

            onOpenRemoveRecordDialog: function (recordId) {
                serviceRecordModel.removeById(recordId);
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
        ServiceRecordScreen {
            onRecordUpdated: function (recordId, serviceRecord) {
                serviceRecordModel.updateRecordById(recordId, serviceRecord);
                stackView.pop();
                console.log("Обновлённая запись" + ServiceRecordBuilder.toJSON(serviceRecord));
            }

            onRecordCreated: function (serviceRecord) {
                serviceRecordModel.append(serviceRecord);
                stackView.pop();
                console.log("Новая запись" + ServiceRecordBuilder.toJSON(serviceRecord));
            }
        }
    }
}
