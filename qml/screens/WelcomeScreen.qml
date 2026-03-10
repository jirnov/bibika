import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import BibikaService

Page {
    id: root

    signal accepted(CarInfo carInfo)
    property CarInfo carInfo:CarInfo{}

    CarModelIndex {
        id: carModelIndex
        Component.onCompleted: loadFromFile(":/cars.json")
    }

    ColumnLayout {
        anchors.fill: parent

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 60

            Item {
                anchors.fill: parent
                Text {
                    anchors.centerIn: parent
                    text: qsTr("Расскажите о своей машине:")
                    font.pixelSize: 22
                    font.bold: true
                }
            }
        }

        SmartTextField {
            id: brandName
            Layout.fillWidth: true
            Layout.leftMargin: 20
            Layout.rightMargin: 20
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
        SmartTextField {
            id: modelName
            Layout.fillWidth: true
            Layout.margins: 20
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
        SmartTextField {
            Layout.fillWidth: true
            Layout.leftMargin: 20
            Layout.rightMargin: 20
            text: String(root.carInfo.lastMileage)
            onEditingFinished: root.carInfo.lastMileage = Number(text) || 0
            placeholderText: qsTr("Текущий пробег (в километрах)")
            digitsOnly: true
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Button {
            Layout.alignment: Qt.AlignCenter
            text: qsTr("Поехали!")
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
