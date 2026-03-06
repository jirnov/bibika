import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import BibikaService

Page {
    id: root
    width: Style.width
    height: Style.height

    signal accepted(CarInfo carInfo)

    property CarInfo carInfo:CarInfo{}

    Label {
        text: qsTr("Welcome!")
        font.pixelSize: Style.fontSizeTitle
        font.bold: true
        color: Style.titleColor
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: scrollView.top
        anchors.bottomMargin: Style.fontSizeTitle
    }

    ScrollView {
        id: scrollView
        background: Rectangle {
            anchors.fill: parent
            color: Style.cardColor
        }
        y: parent.height / 3 - height / 2
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Style.defaultMargin
        contentWidth: availableWidth
        contentHeight: columnLayout.implicitHeight

        ColumnLayout {
            id: columnLayout
            anchors.fill: parent
            anchors.margins: Style.defaultMargin
            spacing: Style.defaultMargin * 2

            Label {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("Enter your vehicle information:")
                horizontalAlignment: Text.AlignHCenter
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: Style.defaultMargin / 2
                Label {
                    text: qsTr("Car brand:")
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
                    text: qsTr("Please fil in the car brand (required filed)")
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
                    text: qsTr("Car model: ")
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
                    text: qsTr("Please fill in the car model (required field)")
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
                    text: qsTr("Mileage (kilometers):")
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
                text: qsTr("Start!")
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
