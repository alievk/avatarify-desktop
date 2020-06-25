import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11

import QtMultimedia 5.15
import Qt.labs.settings 1.0
import Qt.labs.folderlistmodel 2.15

ApplicationWindow {
    id: window
    visible: true
    width: 760
    height: 450
//    title: "Avatarify Desktop"

//    Settings {
//        property alias x: window.x
//        property alias y: window.y
//        property alias width: window.width
//        property alias height: window.height
    //    }

    ColumnLayout {
        anchors.fill: parent
        anchors.rightMargin: 5
        anchors.topMargin: 0
        anchors.leftMargin: 5
        anchors.bottomMargin: 5

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                anchors.fill: parent

                GroupBox {
                    id: settingsGroup
                    width: 270
                    Layout.maximumWidth: 270
                    Layout.preferredWidth: 270
                    Layout.minimumWidth: 270
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    title: qsTr("Settings")

                    Settings {
                        category: "Input"
                        property alias cam_id: cameraSelector.currentValue
                        property alias mirror: cameraMirrorCheckbox.checked
                        property alias smartCrop: cameraSmartCropCheckbox.checked
                        property alias vcam: cameraVCamCheckbox.checked
                    }

                    ColumnLayout {
                        anchors.fill: parent
                        visible: true
                        anchors.bottom: parent.bottom
                        anchors.top: parent.top

                        Label {
                            text: "Choose your camera:"
                        }
                        ComboBox {
                            id: cameraSelector
                            Layout.fillWidth: true

                            model: QtMultimedia.availableCameras
                            textRole: 'displayName'
                        }

                        Switch {
                            id: cameraMirrorCheckbox
                            Layout.fillWidth: true
                            text: qsTr("Flip video preview")
                        }

                        Switch {
                            id: cameraSmartCropCheckbox
                            Layout.fillWidth: true
                            text: qsTr("Smart crop")
                        }

                        Switch {
                            id: cameraVCamCheckbox
                            Layout.fillWidth: true
                            text: qsTr("Virtual camera")
                        }
//                        Label {
//                            text: "You can use Avatarify as a \nvirtual camera for Skype/Zoom calls"
//                            wrapMode: Text.WordWrap
//                        }
                    }
                }

                GroupBox {
                    id: previewGroup
                    title: qsTr("Preview")
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent

                        Label {
                            id: previewScene
                            height: 256
                            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
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

        GroupBox {
            id: avatarGroup
            Layout.fillWidth: true
            title: qsTr("Avatars")

            Settings {
                category: "Input"
                property alias avatar: avatarSelector.currentValue
            }

            ColumnLayout {
                anchors.fill: parent

                Label {
                    text: "Choose your avatar:"
                }
                ComboBox {
                    id: avatarSelector
                    Layout.fillWidth: true

                    model: FolderListModel {
                        folder: "file:///Users/vlivashkin/.avatarify/avatars"
                        nameFilters: ['*.jpg', '*.jpeg', '*.png']
                    }
                    textRole: 'fileBaseName'
                }
            }
        }
    }
}
