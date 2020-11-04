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
        var points = faceFinder.findFace(src);
        if (points.length >= 4) {
            console.log(points);
            console.log(points.length);
            cropWindow.show();
            var xx = points[0] * imageToCrop.width;
            var yy = points[1]  * imageToCrop.height;
            var sx = (points[2] - points[0]);
            var sy = (points[3] - points[1]);
            var ww = sx * imageToCrop.width;
            var hh = sy * imageToCrop.height;
            yy -= Math.min(yy, (0.15 * hh))
            var s = 1.0 / Math.max(sx, sy);
            console.log(sx, sy);
            scaleSlider.value = s;
            flickable.contentX = xx * s;
            flickable.contentY = yy * s;
            scaleSlider.value *= 0.7;
        } else {
            scaleSlider.value = 1.0;
            flickable.contentX = 0;
            flickable.contentY = 0;
        }
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
            //scale: scaleSlider.value
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
        onValueChanged: {
            var s = imageToCrop.scale;
            var w = flickable.width / 2;
            var h = flickable.height / 2;
            flickable.contentX = ((flickable.contentX + w) * (value / s) - w);
            flickable.contentY = ((flickable.contentY + h) * (value / s) - h);
            imageToCrop.scale = value;
        }
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
