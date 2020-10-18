import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Window {
    id: cropWindow
    minimumWidth: 420
    minimumHeight: 465
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight
    width: minimumWidth
    height: minimumHeight
    title: "Crop your image"
    function open(src) {
        imageToCrop.source = src;
        cropWindow.show();
    }
    readonly property var letters: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"]
    function getRandomName() {
        var retVal = "";
        for (var i = 0; i < 16; ++i) {
            retVal += letters[Math.floor(Math.random() * 16)];
        }
        return retVal;
    }

    Flickable {
        id: flickable
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            margins: 10
        }
        width: 400
        height: 400
        contentWidth: imageToCrop.width * imageToCrop.scale
        contentHeight: imageToCrop.height * imageToCrop.scale
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        Image {
            id: imageToCrop
            transformOrigin: Item.TopLeft
            property var proportion: sourceSize.width / sourceSize.height
            width: proportion <= 1.0 ? flickable.width : flickable.width * proportion
            height: proportion > 1.0 ? flickable.height : flickable.height / proportion
            fillMode: Image.PreserveAspectFit
            scale: scaleSlider.value
        }
    }
    Slider {
        id: scaleSlider
        from: 1.0
        to: 10.0
        value: 1.0
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: buttonOpen.left
            margins: 10
        }
        orientation: Qt.Horizontal
    }
    Button {
        id: buttonOpen
        anchors {
            right: buttonSave.left
            bottom: parent.bottom
            margins: 10
        }
        text: "Open"
        onClicked: {
            addImageDialog.open();
        }
    }
    Button {
        id: buttonSave
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 10
        }
        text: "Save"
        onClicked: {
            flickable.grabToImage(function(result) {
                result.saveToFile(manager.rootFolder + "/.avatarify/avatars/" + getRandomName() + ".png");
                console.log("Saving done");
                cropWindow.close();
            });
        }
    }
}
