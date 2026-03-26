pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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

        var record1 = ServiceRecordBuilder.createEmpty(root);
        record1.name = "Замена масла и фильтров и коробки передач";
        record1.price = 500000;
        record1.notes = "Масло 5W-30, фильтр Mann и очень длинная заметка, например стих";
        record1.mileage = 0;
        record1.serviceDate = new Date(2026, 2, 1);  // Март 2026 (месяцы с 0)
        record1.eventType = ServiceRecord.Maintenance;
        record1.hasRepeatAfterDistance = true;
        record1.repeatAfterDistance = 15000;
        record1.hasRepeatAfterMonths = true;
        record1.repeatAfterMonths = 12;
        serviceRecordModel.append(record1);

        var record2 = ServiceRecordBuilder.createEmpty(root);
        record2.name = "Замена стартера";
        record2.price = 20000;
        record2.mileage = 5000;
        record2.notes = "Щётки износились";
        record2.serviceDate = new Date(2026, 2, 2);  // 2 марта 2026
        record2.eventType = ServiceRecord.Repair;
        serviceRecordModel.append(record2);

        var record3 = ServiceRecordBuilder.createEmpty(root);
        record3.name = "Мойка и химчистка";
        record3.price = 200;
        record3.mileage = 5500;
        record3.notes = "Детейлинг салона";
        record3.serviceDate = new Date(2026, 2, 1);  // 1 марта 2026
        record3.eventType = ServiceRecord.Service;
        serviceRecordModel.append(record3);

        var record4 = ServiceRecordBuilder.createEmpty(root);
        record4.name = "Замена масла";
        record4.price = 500000;
        record4.mileage = 10000;
        record4.notes = "Масло Mobil 1 5W-30";
        record4.eventType = ServiceRecord.Repair;
        serviceRecordModel.append(record4);

        var record5 = ServiceRecordBuilder.createEmpty(root);
        record5.name = "Замена АКБ";
        record5.price = 15000;
        record5.mileage = 0;
        record5.notes = "Varta 60Ah";
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
            }
            GradientStop {
                position: 1.0
                color: "#F1F5F9"
            }
        }
    }

    Item {
        id: topPanel
        width: parent.width
        height: 180

        Rectangle {
            x: -1
            y: 0
            width: parent.width + 2
            height: 60
            color: "transparent"
            border.color: "#E5E7EB"
        }

        Text {
            x: 28
            y: 90
            width: parent.width - 10
            elide: Text.ElideRight
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
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        clip: true
        spacing: 20

        model: serviceRecordProxy
        delegate: Rectangle {
            id: delegateRoot
            required property var model
            required property int index

            width: listView.width
            height: 160
            color: "white"
            border.color: "#e0e0e0"
            border.width: 1
            radius: 20

            Item {
                anchors.fill: parent

                Image {
                    source: "/icons/edit.svg"
                    sourceSize.width: 32
                    sourceSize.height: 32
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    anchors.top: parent.top
                    anchors.topMargin: 20

                    MouseArea {
                        width: 48
                        height: 48
                        anchors.centerIn: parent
                        onClicked: root.openEditRecordDialog(delegateRoot.model.recordId)

                        Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                            border.color: "red"
                            border.width: 2
                            visible: false
                        }
                    }
                }

                Image {
                    source: "/icons/trash.svg"
                    sourceSize.width: 32
                    sourceSize.height: 32
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 20

                    MouseArea {
                        width: 48
                        height: 48
                        anchors.centerIn: parent
                        onClicked: root.openRemoveRecordDialog(delegateRoot.model.recordId)

                        Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                            border.color: "red"
                            border.width: 2
                            visible: false
                        }
                    }
                }

                Row {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.margins: 20
                    width: parent.width
                    spacing: 10

                    Rectangle {
                        y: 2
                        width: 16
                        height: width
                        radius: width / 2
                        color: {
                            console.log(delegateRoot.model.eventType);
                            switch (delegateRoot.model.eventType) {
                            case "Maintenance":
                                return "#3B82F6";
                            case "Repair":
                                return "#EF4444";
                            case "Service":
                                return "#10B981";
                            default:
                                return "red";
                            }
                        }
                    }

                    Label {
                        width: parent.width - 100
                        text: delegateRoot.model.name
                        elide: Text.ElideRight
                        font.pointSize: 16
                        font.bold: true
                        font.family: "sans-serif"
                    }
                }

                Label {
                    x: 20
                    y: 65
                    text: Qt.formatDate(delegateRoot.model.serviceDate, "dd.MM.yyyy")
                    font.pointSize: 14
                    font.family: "sans-serif"
                    color: "#6B7280"
                }

                Label {
                    x: 20
                    y: 95
                    text: delegateRoot.model.mileage + "км"
                    font.pointSize: 14
                    font.family: "sans-serif"
                    color: "#6B7280"
                }

                Label {
                    x: 20
                    y: 125
                    text: delegateRoot.model.notes
                    width: parent.width - 70
                    elide: Text.ElideRight
                    font.pointSize: 12
                    font.family: "sans-serif"
                    color: "#9CA3AF"
                }
            }
        }

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
            width: 5
        }
    }

    Rectangle {
        id: addButton
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 30
        z: 1000

        width: 76
        height: width
        radius: width / 2
        color: "#E5E7EB"
        border.color: "#9CA3AF"

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
