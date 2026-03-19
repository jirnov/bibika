import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import BibikaService

Page {
    id: root
    width: parent ? parent.width : AppSettings.width
    height: parent ? parent.height : AppSettings.height

    signal accepted(CarInfo carInfo)
    property CarInfo carInfo: CarInfoBuilder.createEmpty(root)

    readonly property int buttonAreaHeight: 100
    readonly property int buttonBottomMargin: 30
    readonly property int fieldSpacing: 16
    readonly property int headerHeight: 60

    CarModelIndex {
        id: carModelIndex
        Component.onCompleted: {
            if (!carModelIndex.loadFromFile(":/cars.json")) {
                console.error("Failed to load cars.json");
            }
        }
    }

    background: Rectangle {
        gradient: Gradient {
            orientation: Gradient.Vertical
            GradientStop {
                position: 0.0
                color: "#F8FAFC"
            }  // Светлый сверху
            GradientStop {
                position: 1.0
                color: "#F1F5F9"
            }  // Чуть темнее снизу
        }
    }

    Flickable {
        anchors.fill: parent
        contentHeight: columnLayout.height + root.buttonAreaHeight
        boundsBehavior: Flickable.OvershootBounds

        ColumnLayout {
            id: columnLayout
            width: parent.width
            spacing: root.fieldSpacing

            Item {
                Layout.preferredHeight: root.fieldSpacing * 3
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: root.headerHeight
                color: "transparent"

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: Style.defaultMargin * 3
                    anchors.right: parent.right
                    anchors.rightMargin: Style.defaultMargin * 3
                    text: qsTr("Расскажите о своей машине:")
                    wrapMode: Text.WordWrap
                    font.pixelSize: 32
                    font.bold: true
                }
            }

            Item {
                Layout.preferredHeight: root.fieldSpacing * 2
            }

            SmartTextField {
                id: brandName
                Layout.fillWidth: true
                Layout.leftMargin: Style.horizontalMargin
                Layout.rightMargin: Style.horizontalMargin
                placeholderText: qsTr("Марка/бренд автомобиля")
                text: root.carInfo.brandName
                suggestions: carModelIndex.brands
                onValidateField: {
                    errorText = root.carInfo.validateBrand();
                }
                onEditingFinished: {
                    root.carInfo.brandName = text;
                    errorText = root.carInfo.validateBrand();
                }
            }

            SmartTextField {
                id: modelName
                property var _updateTimer: Timer {
                    interval: 300
                    repeat: false
                    onTriggered: modelName.suggestions = carModelIndex.getModelsForBrand(root.carInfo.brandName)
                }
                Layout.fillWidth: true
                Layout.leftMargin: Style.horizontalMargin
                Layout.rightMargin: Style.horizontalMargin
                text: root.carInfo.modelName
                placeholderText: qsTr("Модель автомобиля")
                onEditingFinished: {
                    root.carInfo.modelName = text;
                }
                onValidateField: {
                    errorText = root.carInfo.validateModel();
                }

                onUpdateSuggestions: {
                    _updateTimer.restart();
                }
            }

            SmartTextField {
                Layout.fillWidth: true
                Layout.leftMargin: Style.horizontalMargin
                Layout.rightMargin: Style.horizontalMargin
                text: String(root.carInfo.mileage)
                onEditingFinished: root.carInfo.mileage = Number(text) || 0
                placeholderText: qsTr("Текущий пробег (в километрах)")
                digitsOnly: true
            }
        }
    }

    AcceptButton {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: Style.horizontalMargin
        }

        enabled: root.carInfo.isValid
        text: qsTr("Поехали!")
        onClicked: {
            root.accepted(root.carInfo);
            console.log(root.carInfo.toJSON());
        }
    }
}
