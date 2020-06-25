import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11

Item {
    id: config
    width: 262
    height: 640

    ColumnLayout {
        id: configLayout
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

        GroupBox {
            id: previewGroup
            Layout.fillWidth: true
            enabled: true
            title: qsTr("Preview")

            ColumnLayout {
                id: previewGroupLayout
                anchors.fill: parent

                Label {
                    id: previewScene
                    height: 256
                    Layout.maximumHeight: 256
                    Layout.maximumWidth: 256
                    Layout.minimumHeight: 256
                    Layout.minimumWidth: 256
                    Layout.fillHeight: false
                    Layout.fillWidth: false
                    Layout.preferredHeight: 256
                    Layout.preferredWidth: 256
                    width: 256
                }

                Label {
                    id: name
                    text: qsTr("Stream: none")
                }

                Label {
                    id: fps
                    text: qsTr("FPS: 0")
                }
            }

        }
    }

}
