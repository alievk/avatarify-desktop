import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11
import Qt.labs.settings 1.0

import Qt.labs.folderlistmodel 2.15

import "content"

ApplicationWindow {
    id: window
    visible: true
    width: 790
    height: 320

//    Settings {
//        property alias x: window.x
//        property alias y: window.y
//        property alias width: window.width
//        property alias height: window.height
//    }

    RowLayout {
        anchors.fill: parent
        anchors.rightMargin: 5
        anchors.bottomMargin: 5
        anchors.topMargin: 0
        anchors.leftMargin: 5

        Config {
            id: config
            Layout.fillHeight: true
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ColumnLayout {
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: parent.top

                GroupBox {
                    id: previewGroup
                    Layout.fillWidth: true
                    enabled: true
                    title: qsTr("Preview")

                    ColumnLayout {
                        anchors.fill: parent

                        Label {
                            id: previewScene
                            height: 256
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            Layout.maximumHeight: 256
                            Layout.maximumWidth: 455
                            Layout.minimumHeight: 256
                            Layout.minimumWidth: 455
                            Layout.fillHeight: false
                            Layout.fillWidth: false
                            Layout.preferredHeight: 256
                            Layout.preferredWidth: 455
                            width: 455
                        }
                    }
                }
            }
        }
    }
}
