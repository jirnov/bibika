pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import BibikaService

Page {
    id: root
    width: AppSettings.width
    height: AppSettings.height

    property string _carName: AppSettings.carInfo.name
    property int _mileage: AppSettings.carInfo.lastMileage
    property date _mileageDate: AppSettings.carInfo.lastMileageDate

    signal openAddRecordDialog()
    signal openEditRecordDialog(int recordId)
    signal openSettingsDialog()
    signal openMileageDialog()

    ServiceRecordModel {
        id: dataModel

        Component.onCompleted: {
            // Очищаем модель перед добавлением
            //prodModel.clear()

            // Запись 1: Замена масла
            var record1 = ServiceRecordBuilder.createEmpty()
            record1.name = "Замена масла"
            record1.notes = "Лить только Liqui Molly"
            record1.price = 4500
            record1.mileage = 0
            record1.serviceDate = new Date(2026, 2, 1)  // Март 2026 (месяцы с 0)
            record1.eventType = ServiceRecord.Maintenance
            record1.hasRepeatAfterDistance = true
            record1.repeatAfterDistance = 15000
            record1.hasRepeatAfterMonths = true
            record1.repeatAfterMonths = 12
            dataModel.append(record1)

            // Запись 2: Замена лобового стекла
            var record2 = ServiceRecordBuilder.createEmpty()
            record2.name = "Замена лобового стекла"
            record2.notes = "Тайваньское"
            record2.price = 20000
            record2.mileage = 5000
            record2.serviceDate = new Date(2026, 2, 2)  // 2 марта 2026
            record2.eventType = ServiceRecord.Repair
            dataModel.append(record2)

            // Запись 3: Покупка омывайки
            var record3 = ServiceRecordBuilder.createEmpty()
            record3.name = "Покупка омывайки"
            record3.notes = "Зелёная, -30"
            record3.price = 200
            record3.mileage = 5500
            record3.serviceDate = new Date(2026, 2, 1)  // 1 марта 2026
            record3.eventType = ServiceRecord.Service
            dataModel.append(record3)

            // Запись 4: Капитальный ремонт двигателя
            var record4 = ServiceRecordBuilder.createEmpty()
            record4.name = "Капитальный ремонт двигателя"
            record4.notes = "Этого лучше избегать!"
            record4.price = 500000
            record4.mileage = 10000
            // Без даты - будет текущая или пустая
            record4.eventType = ServiceRecord.Repair
            dataModel.append(record4)

            // Запись 5: Замена масла в коробке
            var record5 = ServiceRecordBuilder.createEmpty()
            record5.name = "Замена масла в коробке"
            record5.notes = ""
            record5.price = 15000
            record5.mileage = 0
            record5.eventType = ServiceRecord.Maintenance
            dataModel.append(record5)

            console.log("Добавлено записей:", dataModel.rowCount())
        }
    }


    Item {
        anchors.fill: parent

        // Верхняя панель с фиксированной высотой
        Column {
            id: topPanel
            width: parent.width
            height: 240  // 4 * 60

            Rectangle {
                width: parent.width
                height: 60
                color: "white"
                border.color: "#eeeeee"
                Text {
                    anchors.centerIn: parent
                    text: qsTr("Главный экран")
                    font.pixelSize: Style.fontSizeTitle
                    font.bold: true
                }
            }

            Rectangle {
                width: parent.width
                height: 60
                color: "white"
                border.color: "#eeeeee"
                Text {
                    anchors.centerIn: parent
                    text: qsTr("Ваш автомобиль: " + root._carName)
                    font.pixelSize: Style.fontSizeDefault
                }
            }

            Rectangle {
                width: parent.width
                height: 60
                color: "white"
                border.color: "#eeeeee"
                Text {
                    anchors.centerIn: parent
                    text: qsTr("Текущий пробег: " + root._mileage)
                    font.pixelSize: Style.fontSizeDefault
                }
            }

            Rectangle {
                width: parent.width
                height: 60
                color: "white"
                border.color: "#eeeeee"
                Text {
                    anchors.centerIn: parent
                    text: qsTr("пробег обновлён: " + Qt.formatDate(root._mileageDate, "dd.MM.yyyy"))
                    font.pixelSize: Style.fontSizeDefault
                }
            }
        }

        // ListView под верхней панелью
        ListView {
            id: listView
            anchors.top: topPanel.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            clip: true
            spacing: 2

            model: dataModel
            delegate: Rectangle {
                required property var model
                required property int index

                width: listView.width
                height: 120
                color: index % 2 === 0 ? "#f8f8f8" : "white"
                border.color: "#e0e0e0"
                border.width: 1
                radius: 4

                Item {
                    anchors.fill: parent
                    anchors.margins: 10

                    Column {
                        Label {
                            text: "Название: " + model.name
                            color: "grey"
                            font.pixelSize: 14
                        }

                        Label {
                            text: "Заметка: " + (model.notes ? model.notes : "(нет)")
                            color: "green"
                            font.pixelSize: 14
                        }

                        Button {
                            text: "Редактировать"

                            onClicked: {
                                root.openEditRecordDialog(model.recordId)
                            }
                        }

                        Button {
                            text: "Удалить"

                            onClicked: {
                                dataModel.removeRecordById(model.recordId)
                            }
                        }
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }
        }
    }
}
