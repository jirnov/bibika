pragma Singleton
import QtQuick

QtObject {
    readonly property color backgroundColor: "#F8FAFC"
    readonly property color cardColor: "white"
    readonly property color titleColor: "#333333"
    readonly property color hintColor: "#ff5252"

    readonly property int horizontalMargin: 16

    // Цвет кнопки
    readonly property color buttonColor: "#3B82F6"
    // Рамка выделенного поля ввода
    readonly property color activeBorderColor: "#EF4444"
    // Надписи в поле ввода вне фокуса
    readonly property color inactiveColor: "#9CA3AF"
    // Заголовок #1D1E2C

    readonly property int defaultMargin: 8

    readonly property int fontSizeTitle: 20
    readonly property int fontSizeHint: 12
    readonly property int fontSizeDefault: 16
}



