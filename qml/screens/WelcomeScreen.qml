import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: welcomeScreen
    width: 288
    height: 640

    ColumnLayout {
        width: parent.width
        Layout.fillWidth: true
        spacing: 4

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "Добро пожаловать!"
            font.pixelSize: 22
            font.bold: true
            color: "#000000"
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.maximumWidth: parent.width - 20
            Layout.leftMargin: 10
            Layout.rightMargin: 10
        }

        Label {
            text: "Введите данные своего автомобиля:"
            Layout.alignment: Qt.AlignHCenter
            font.pixelSize: 14
            color: "#000000"
            opacity: 0.9
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.maximumWidth: parent.width - 20
            Layout.leftMargin: 10
            Layout.rightMargin: 10
        }

        Rectangle {
            height: 200
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            //color: "#f9f9fb"
            border.color: "#e9e9e9"
            radius: 16

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                uniformCellSizes: false
                spacing: 0

                ColumnLayout {

                    Label {
                        text: "Марка автомобиля:"
                    }

                    TextField {
                        id: brandField
                        placeholderText: "Seat, Toyota, Bmw..."

                        Binding on text {
                            value: settings.brand
                            when: settings.brand.length > 0
                        }
                    }
                }

                ColumnLayout {
                    Label {
                        text: "Модель автомобиля:"
                    }
                    TextField {
                        id: modelField
                        placeholderText: "Ateca, Camry, X3..."

                        Binding on text {
                            value: settings.model
                            when: settings.model.length > 0
                        }
                    }
                }

                ColumnLayout {
                    Label {
                        text: "Пробег в км:"
                    }
                    TextField {
                        id: milestoneField

                        Binding on text {
                            value: String(settings.mileage)
                            when: settings.mileage > 0
                        }

                        validator: IntValidator {
                            bottom: 0
                        }
                    }
                }
            }
        }
        Button {
            Layout.alignment: Qt.AlignHCenter
            text: "Продолжить"
            enabled: brandField.text.length > 0 && modelField.text.length > 0
                     && milestoneField.text.length > 0
            onClicked: {
                settings.brand = brandField.text
                settings.model = modelField.text
                settings.mileage = parseInt(milestoneField.text)
                settings.useWelcomeScreen = useWelcomeScreenCheckbox.checked
                rootScreen.replaceCurrentItem(dashboardScreen)
            }
        }

        CheckBox {
            Layout.alignment: Qt.AlignHCenter
            id: useWelcomeScreenCheckbox
            text: "Показывать при запуске"
            checked: settings.useWelcomeScreen
        }
    }
}
