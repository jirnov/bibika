import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material 2.15

Item {
    id: welcomeScreen

    // Фоновый градиент
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            orientation: Gradient.Vertical
            GradientStop { position: 0.0; color: "#1a237e" }
            GradientStop { position: 0.5; color: "#283593" }
            GradientStop { position: 1.0; color: "#3949ab" }
        }
    }

    // Декоративные элементы (уменьшены для маленького экрана)
    Rectangle {
        width: parent.width * 1.2
        height: width
        radius: width / 2
        color: "#80ffffff"
        opacity: 0.1
        x: -width * 0.3
        y: -height * 0.3
    }

    Flickable {
        anchors.fill: parent
        contentHeight: contentColumn.height + 40
        clip: true
        boundsBehavior: Flickable.OvershootBounds
        
        ColumnLayout {
            id: contentColumn
            width: parent.width
            spacing: 16

            // Верхний отступ
            Item { Layout.preferredHeight: 10 }

            // Иконка (уменьшена)
            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                width: 70
                height: 70
                radius: 35
                color: "#fff8e1"
                
                Text {
                    anchors.centerIn: parent
                    text: "🚗"
                    font.pixelSize: 32
                }
            }

            // Заголовки (уменьшены)
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Добро пожаловать!"
                    font.pixelSize: 22
                    font.bold: true
                    color: "white"
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    Layout.maximumWidth: parent.width - 20
                    Layout.leftMargin: 10
                    Layout.rightMargin: 10
                }

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Введите данные автомобиля"
                    font.pixelSize: 14
                    color: "#e0e0e0"
                    opacity: 0.9
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    Layout.maximumWidth: parent.width - 20
                    Layout.leftMargin: 10
                    Layout.rightMargin: 10
                }
            }

            // Карточка с формой (без тени для экономии места)
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 380
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                color: "white"
                radius: 16

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    // Поля ввода
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 16

                        // Марка
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 3

                            Label {
                                text: "Марка"
                                font.pixelSize: 12
                                font.bold: true
                                color: "#424242"
                                leftPadding: 4
                            }

                            TextField {
                                id: brandField
                                Layout.fillWidth: true
                                placeholderText: "Toyota, BMW..."
                                
                                background: Rectangle {
                                    radius: 8
                                    border.color: parent.activeFocus ? "#2196F3" : "#e0e0e0"
                                    border.width: parent.activeFocus ? 2 : 1
                                    color: parent.hovered ? "#f5f5f5" : "white"
                                }
                                
                                font.pixelSize: 14
                                leftPadding: 12
                                rightPadding: 12
                                height: 42

                                Binding on text {
                                    value: settings.brand
                                    when: settings.brand.length > 0
                                }
                            }
                        }

                        // Модель
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 3

                            Label {
                                text: "Модель"
                                font.pixelSize: 12
                                font.bold: true
                                color: "#424242"
                                leftPadding: 4
                            }

                            TextField {
                                id: modelField
                                Layout.fillWidth: true
                                placeholderText: "Camry, X5..."
                                
                                background: Rectangle {
                                    radius: 8
                                    border.color: parent.activeFocus ? "#2196F3" : "#e0e0e0"
                                    border.width: parent.activeFocus ? 2 : 1
                                    color: parent.hovered ? "#f5f5f5" : "white"
                                }
                                
                                font.pixelSize: 14
                                leftPadding: 12
                                rightPadding: 12
                                height: 42

                                Binding on text {
                                    value: settings.model
                                    when: settings.model.length > 0
                                }
                            }
                        }

                        // Пробег
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 3

                            Label {
                                text: "Пробег (км)"
                                font.pixelSize: 12
                                font.bold: true
                                color: "#424242"
                                leftPadding: 4
                            }

                            TextField {
                                id: milestoneField
                                Layout.fillWidth: true
                                placeholderText: "0"
                                
                                background: Rectangle {
                                    radius: 8
                                    border.color: parent.activeFocus ? "#2196F3" : "#e0e0e0"
                                    border.width: parent.activeFocus ? 2 : 1
                                    color: parent.hovered ? "#f5f5f5" : "white"
                                }
                                
                                font.pixelSize: 14
                                leftPadding: 12
                                rightPadding: 12
                                height: 42

                                validator: IntValidator {
                                    bottom: 0
                                    top: 9999999
                                }

                                Binding on text {
                                    value: String(settings.mileage)
                                    when: settings.mileage > 0
                                }
                            }
                        }
                    }

                    // Разделитель
                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: "#e0e0e0"
                        Layout.topMargin: 4
                        Layout.bottomMargin: 4
                    }

                    // Чекбокс (уменьшен)
                    CheckBox {
                        id: useWelcomeScreenCheckbox
                        text: "Показывать при запуске"
                        checked: settings.useWelcomeScreen
                        
                        spacing: 8
                        
                        indicator: Rectangle {
                            implicitWidth: 20
                            implicitHeight: 20
                            x: useWelcomeScreenCheckbox.leftPadding
                            y: parent.height / 2 - height / 2
                            radius: 4
                            border.color: parent.activeFocus ? "#2196F3" : "#bdbdbd"
                            border.width: parent.hovered ? 2 : 1
                            color: parent.checked ? "#2196F3" : "white"

                            Rectangle {
                                width: 12
                                height: 12
                                anchors.centerIn: parent
                                radius: 2
                                color: "white"
                                visible: parent.checked
                            }
                        }

                        contentItem: Text {
                            text: useWelcomeScreenCheckbox.text
                            font.pixelSize: 12
                            color: "#616161"
                            leftPadding: useWelcomeScreenCheckbox.indicator.width + useWelcomeScreenCheckbox.spacing
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    // Кнопка продолжения
                    Button {
                        id: continueButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 48
                        text: "Продолжить"
                        
                        enabled: brandField.text.length > 0 && 
                                modelField.text.length > 0 && 
                                milestoneField.text.length > 0 && 
                                parseInt(milestoneField.text) >= 0

                        background: Rectangle {
                            radius: 10
                            color: parent.enabled ? 
                                  (parent.hovered ? "#1565C0" : "#1976D2") : 
                                  "#bdbdbd"
                        }

                        contentItem: Text {
                            text: parent.text
                            font.pixelSize: 16
                            font.bold: true
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            settings.brand = brandField.text
                            settings.model = modelField.text
                            settings.mileage = parseInt(milestoneField.text)
                            settings.useWelcomeScreen = useWelcomeScreenCheckbox.checked
                            rootScreen.replaceCurrentItem(dashboardScreen)
                        }
                    }
                }
            }

            // Нижний текст
            Label {
                Layout.alignment: Qt.AlignHCenter
                Layout.bottomMargin: 10
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                text: "Данные сохраняются локально"
                font.pixelSize: 10
                color: "#e0e0e0"
                opacity: 0.7
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}