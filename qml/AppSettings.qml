pragma Singleton
import QtCore
import QtQuick
import BibikaService

QtObject {
    id: root

    readonly property int width: 576
    readonly property int height: 1280

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
        root._persistent.carInfoJson = newCarInfo.toJSON();
        root._persistent.sync();
    }
}
