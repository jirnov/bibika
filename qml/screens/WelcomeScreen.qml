import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import BibikaService

Page {
    id: root
    width: Style.width
    height: Style.height

    signal accepted(CarInfo carInfo)

    required property CarInfo carInfo

    header: ToolBar {
        ColumnLayout {
            anchors.fill: parent
            Label {
                text: "Добро пожаловать!"
                font.pixelSize: Style.fontSizeTitle
                font.bold: true
                color: Style.titleColor
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    ScrollView {
        background: Rectangle {
            anchors.fill: parent
            color: Style.cardColor
        }
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Style.defaultMargin
        contentWidth: availableWidth

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Style.defaultMargin
            spacing: Style.defaultMargin * 2

            Label {
                Layout.fillWidth: true
                text: "Введите данные своего автомобиля:"
                horizontalAlignment: Text.AlignHCenter
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: Style.defaultMargin / 2
                Label {
                    text: "Марка автомобиля:"
                    font.pixelSize: Style.fontSizeLabel
                }

                TextField {
                    Layout.fillWidth: true
                    id: carBrandInput
                    text: root.carInfo.brandName
                    placeholderText: "Лада, Toyota, Cupra..."
                    font.pixelSize: Style.fontSizeDefault
                    onTextEdited: root.carInfo.brandName = text
                    onEditingFinished: carBrandInputHint.visible = !root.carInfo.isBrandNameValid
                }
                Label {
                    id: carBrandInputHint
                    Layout.fillWidth: true
                    color: Style.hintColor
                    font.pixelSize: Style.fontSizeHint
                    visible: false
                    text: "Поле не может быть пустым, введите название бренда"
                    opacity: visible ? 1 : 0
                    Behavior on opacity {
                        NumberAnimation{duration: 200}
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: Style.defaultMargin / 2
                Label {
                    text: "Модель автомобиля:"
                    font.pixelSize: Style.fontSizeLabel
                }
                TextField {
                    Layout.fillWidth: true
                    id: carModelInput
                    text: root.carInfo.modelName
                    placeholderText: "Гранта, Camry, Leon..."
                    font.pixelSize: Style.fontSizeDefault
                    onTextEdited: root.carInfo.modelName = text
                    onEditingFinished: carModelInputHint.visible = !root.carInfo.isModelNameValid
                }
                Label {
                    id: carModelInputHint
                    Layout.fillWidth: true
                    color: Style.hintColor
                    font.pixelSize: Style.fontSizeHint
                    text: "Поле не может быть пустым, введите название модели"
                    visible: false
                    opacity: visible ? 1 : 0
                    Behavior on opacity {
                        NumberAnimation{duration: 200}
                    }
                }
            }

            ColumnLayout {
                spacing: Style.defaultMargin / 2
                Label {
                    text: "Пробег в км:"
                    font.pixelSize: Style.fontSizeLabel
                }
                TextField {
                    id: milestoneField
                    font.pixelSize: Style.fontSizeDefault
                    text: String(root.carInfo.lastMileage)
                    onTextEdited: root.carInfo.lastMileage = Number(text) || 0
                    validator: IntValidator {
                        bottom: 0
                    }
                }
            }

            Button {
                text: "продолжить"
                highlighted: true
                Layout.alignment: Qt.AlignCenter
                Layout.bottomMargin: Style.defaultMargin * 2
                enabled: root.carInfo.isValid
                onClicked: {
                    if (root.carInfo.isValid) {
                        root.accepted(root.carInfo)
                        root.visible = false
                        console.log(root.carInfo.toJSON())
                    }
                }
            }
        }
    }
}
