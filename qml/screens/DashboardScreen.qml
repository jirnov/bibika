pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls

import BibikaService

Page {
    id: root
    width: parent ? parent.width : AppSettings.width
    height: parent ? parent.height : AppSettings.height

    property string _carName: AppSettings.carInfo.name
    property int _mileage: AppSettings.carInfo.mileage
    property date _mileageUpdateDate: AppSettings.carInfo.mileageUpdateDate

    signal openAddRecordDialog
    signal openEditRecordDialog(int recordId)
    signal openSettingsDialog
    signal openMileageDialog
    signal openRemoveRecordDialog(int recordId)

    Component.onCompleted: {
        serviceRecordModel.clear();

        // Запись 1: Замена масла
        var record1 = ServiceRecordBuilder.createEmpty(root);
        record1.name = "Первая запись";
        record1.price = 500000;
        record1.mileage = 0;
        record1.serviceDate = new Date(2026, 2, 1);  // Март 2026 (месяцы с 0)
        record1.eventType = ServiceRecord.Maintenance;
        record1.hasRepeatAfterDistance = true;
        record1.repeatAfterDistance = 15000;
        record1.hasRepeatAfterMonths = true;
        record1.repeatAfterMonths = 12;
        serviceRecordModel.append(record1);

        // Запись 2: Замена лобового стекла
        var record2 = ServiceRecordBuilder.createEmpty(root);
        record2.name = "Вторая запись";
        record2.price = 20000;
        record2.mileage = 5000;
        record2.serviceDate = new Date(2026, 2, 2);  // 2 марта 2026
        record2.eventType = ServiceRecord.Repair;
        serviceRecordModel.append(record2);

        // Запись 3: Покупка омывайки
        var record3 = ServiceRecordBuilder.createEmpty(root);
        record3.name = "Третья запись";
        record3.price = 200;
        record3.mileage = 5500;
        record3.serviceDate = new Date(2026, 2, 1);  // 1 марта 2026
        record3.eventType = ServiceRecord.Service;
        serviceRecordModel.append(record3);

        // Запись 4: Капитальный ремонт двигателя
        var record4 = ServiceRecordBuilder.createEmpty(root);
        record4.name = "Четвертая запись";
        record4.price = 500000;
        record4.mileage = 10000;
        // Без даты - будет текущая или пустая
        record4.eventType = ServiceRecord.Repair;
        serviceRecordModel.append(record4);

        // Запись 5: Замена масла в коробке
        var record5 = ServiceRecordBuilder.createEmpty(root);
        record5.name = "Пятая запись";
        record5.price = 15000;
        record5.mileage = 0;
        record5.eventType = ServiceRecord.Maintenance;
        serviceRecordModel.append(record5);

        console.log("Всего записей:", serviceRecordModel.count());
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

    Item {
        id: topPanel
        width: parent.width
        height: 180  // Фиксированная высота

        Rectangle {
            x: -1
            y: 0
            width: parent.width + 2
            height: 80
            color: "transparent"
            border.color: "#E5E7EB"
        }

        Text {
            x: 28
            y: 130
            text: root._carName
            font.pixelSize: 30
            font.bold: true
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

        model: serviceRecordProxy
        delegate: Rectangle {
            id: delegateRoot
            required property var model
            required property int index

            width: listView.width
            height: 160
            color: index % 2 === 0 ? "#f8f8f8" : "white"
            border.color: "#e0e0e0"
            border.width: 1
            radius: 4

            Item {
                anchors.fill: parent
                anchors.margins: 10

                Column {
                    Row {
                        spacing: 5
                        Label {
                            text: "Название: " + delegateRoot.model.name
                            color: "grey"
                            font.pixelSize: 14
                        }

                        Label {
                            text: "Тип: " + delegateRoot.model.eventType
                        }
                    }
                    Row {
                        spacing: 5
                        Label {
                            text: "Заметка: " + (delegateRoot.model.notes ? delegateRoot.model.notes : "(нет)")
                            font.pixelSize: 14
                        }

                        Label {
                            text: "Цена: " + delegateRoot.model.price
                            font.pixelSize: 14
                        }

                        Label {
                            text: "Пробег: " + delegateRoot.model.mileage
                            font.pixelSize: 14
                        }
                    }

                    Button {
                        text: "Редактировать"

                        onClicked: {
                            root.openEditRecordDialog(delegateRoot.model.recordId);
                        }
                    }

                    Button {
                        text: "Удалить"

                        onClicked: {
                            root.openRemoveRecordDialog(delegateRoot.model.recordId);
                        }
                    }
                }
            }
        }

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
        }
    }

    Rectangle {
        id: addButton
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        z: 1000

        width: 56
        height: width
        radius: width / 2
        color: "#9CA3AF"

        Label {
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -5
            text: "+"
            font.pixelSize: 42
        }

        MouseArea {
            anchors.fill: parent
            onClicked: root.openAddRecordDialog()
        }
    }
}
