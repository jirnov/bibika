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
        ServiceRecordModel.clear();

        // Запись 1: Замена масла
        var record1 = ServiceRecordBuilder.createEmpty(root);
        record1.name = "Замена масла";
        record1.notes = "Лить только Liqui Molly";
        record1.price = 4500;
        record1.mileage = 0;
        record1.serviceDate = new Date(2026, 2, 1);  // Март 2026 (месяцы с 0)
        record1.eventType = ServiceRecord.Maintenance;
        record1.hasRepeatAfterDistance = true;
        record1.repeatAfterDistance = 15000;
        record1.hasRepeatAfterMonths = true;
        record1.repeatAfterMonths = 12;
        ServiceRecordModel.append(record1);

        // Запись 2: Замена лобового стекла
        var record2 = ServiceRecordBuilder.createEmpty(root);
        record2.name = "Замена лобового стекла";
        record2.notes = "Тайваньское";
        record2.price = 20000;
        record2.mileage = 5000;
        record2.serviceDate = new Date(2026, 2, 2);  // 2 марта 2026
        record2.eventType = ServiceRecord.Repair;
        ServiceRecordModel.append(record2);

        // Запись 3: Покупка омывайки
        var record3 = ServiceRecordBuilder.createEmpty(root);
        record3.name = "Покупка омывайки";
        record3.notes = "Зелёная, -30";
        record3.price = 200;
        record3.mileage = 5500;
        record3.serviceDate = new Date(2026, 2, 1);  // 1 марта 2026
        record3.eventType = ServiceRecord.Service;
        ServiceRecordModel.append(record3);

        // Запись 4: Капитальный ремонт двигателя
        var record4 = ServiceRecordBuilder.createEmpty(root);
        record4.name = "Капитальный ремонт двигателя";
        record4.notes = "Этого лучше избегать!";
        record4.price = 500000;
        record4.mileage = 10000;
        // Без даты - будет текущая или пустая
        record4.eventType = ServiceRecord.Repair;
        ServiceRecordModel.append(record4);

        // Запись 5: Замена масла в коробке
        var record5 = ServiceRecordBuilder.createEmpty(root);
        record5.name = "Замена масла в коробке";
        record5.notes = "";
        record5.price = 15000;
        record5.mileage = 0;
        record5.eventType = ServiceRecord.Maintenance;
        ServiceRecordModel.append(record5);

        console.log("Всего записей:", ServiceRecordModel.count());
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
        anchors.fill: parent

        Rectangle {
            x: -1
            y: 0
            width: parent.width + 2
            height: 80
            color: "transparent"
            border.color: "#E5E7EB"
            //border.color: "black"
        }

        Text {
            x: 28
            y: 130
            text: root._carName
            font.pixelSize: 30
            font.bold: true
        }

        // Верхняя панель с фиксированной высотой
        Column {
            id: topPanel
            width: parent.width
            height: 300  // 4 * 60

            /*
            Rectangle {
                x: 28
                y: 0
                width: parent.width
                height: 60
                color: "red"
                border.color: "#eeeeee"
                Text {
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }
                    text: root._carName
                    font.pixelSize: 36
                }
            }
            */
            /*
            Text {
                anchors {
                    left: parent.left
                    leftMargin: 28
                    verticalCenter: parent.verticalCenter
                }
                text: root._carName
                font.pixelSize: 36
                font.bold: true
            }
            */
            /*
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
                    text: qsTr("пробег обновлён: " + Qt.formatDate(root._mileageUpdateDate, "dd.MM.yyyy"))
                    font.pixelSize: Style.fontSizeDefault
                }
            }
*/
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

            model: ServiceRecordModel
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

                        Label {
                            text: "Заметка: " + (delegateRoot.model.notes ? delegateRoot.model.notes : "(нет)")
                            color: "green"
                            font.pixelSize: 14
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
    }

    Rectangle {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 20

        width: 100
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
