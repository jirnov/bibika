import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import BibikaService

Page {
    id: root
    width: AppSettings.width
    height: AppSettings.height

    signal accepted(CarInfo carInfo)
    property CarInfo carInfo:CarInfoBuilder.createEmpty()

    readonly property int buttonAreaHeight: 100
    readonly property int buttonBottomMargin: 30
    readonly property int fieldSpacing: 20
    readonly property int headerHeight: 60

    CarModelIndex {
        id: carModelIndex
        Component.onCompleted: loadFromFile(":/cars.json")
    }

    Flickable {
        anchors.fill: parent
        contentHeight: columnLayout.height + buttonAreaHeight
        boundsBehavior: Flickable.OvershootBounds

        ColumnLayout {
            id: columnLayout
            anchors.fill: parent
            spacing: 0

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: headerHeight
                color: "white"
                border.color: "#EEEEEE"

                Text {
                    anchors.centerIn: parent
                    text: qsTr("Расскажите о своей машине:")
                    font.pixelSize: Style.fontSizeTitle
                    font.bold: true
                }
            }

            Item {
                Layout.fillWidth: true
                height: fieldSpacing
            }

            SmartTextField {
                id: brandName
                Layout.fillWidth: true
                Layout.leftMargin: fieldSpacing
                Layout.rightMargin: fieldSpacing
                placeholderText: qsTr("Марка/бренд автомобиля")
                text: root.carInfo.brandName
                suggestions: carModelIndex.brands
                onValidateField: {
                    errorText = root.carInfo.validateBrand()
                }
                onEditingFinished: {
                    root.carInfo.brandName = text
                    errorText = root.carInfo.validateBrand()
                }
            }

            Item {
                Layout.fillWidth: true
                height: fieldSpacing
            }

            SmartTextField {
                id: modelName
                Layout.fillWidth: true
                Layout.leftMargin: fieldSpacing
                Layout.rightMargin: fieldSpacing
                text: root.carInfo.modelName
                placeholderText: qsTr("Модель автомобиля")
                onEditingFinished: {
                    root.carInfo.modelName = text
                }
                onValidateField: {
                    errorText = root.carInfo.validateModel()
                }

                onUpdateSuggestions: {
                    modelName.suggestions = carModelIndex.getModelsForBrand(root.carInfo.brandName)
                }
            }

            Item {
                Layout.fillWidth: true
                height: fieldSpacing
            }

            SmartTextField {
                Layout.fillWidth: true
                Layout.leftMargin: fieldSpacing
                Layout.rightMargin: fieldSpacing
                text: String(root.carInfo.lastMileage)
                onEditingFinished: root.carInfo.lastMileage = Number(text) || 0
                placeholderText: qsTr("Текущий пробег (в километрах)")
                digitsOnly: true
            }

            Item {
                Layout.fillHeight: true
                Layout.minimumHeight: fieldSpacing
            }
        }
    }

    Rectangle {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: 100
        color: "white"
        border.color: "#EEEEEE"

        Button {
            id: acceptButton
            enabled: root.carInfo.isValid
            opacity: enabled ? 1.0 : 0.5
            Behavior on opacity { NumberAnimation { duration: 200 } }
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                margins: 20
                bottomMargin: buttonBottomMargin
            }
            height: 56

            text: qsTr("Поехали!")
            onClicked: {
                root.accepted(root.carInfo)
                root.visible = false
                console.log(root.carInfo.toJSON())
            }
        }
    }

}
