pragma Singleton
import QtCore
import QtQuick
import BibikaService

QtObject {
    id: root

    readonly property real _aspectRatio: 1080 / 2340.0
    readonly property int width: 375
    readonly property int height: width / _aspectRatio

    property int keyboardHeight: 0

    function setKeyboardHeight(newHeight) {
        keyboardHeight = newHeight;
    }

    property Settings _persistent: Settings {
        property string carInfoJson: ""
        property alias showWelcomeScreen: root.showWelcomeScreen
    }

    property bool showWelcomeScreen: true
    readonly property CarInfo carInfo: CarInfoBuilder.fromJSON(root._persistent.value("carInfoJson"), root)

    function updateCarInfo(newCarInfo) {
        root._persistent.carInfoJson = CarInfoBuilder.toJSON(newCarInfo);
        root._persistent.sync();
    }
}
