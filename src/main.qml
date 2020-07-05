import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11

import QtMultimedia 5.15
import Qt.labs.settings 1.0
import Qt.labs.folderlistmodel 2.15

import com.avatarify.desktop 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 760
    height: 530
    title: "Avatarify Desktop"

    Settings {
        id: window_settings
        category: "Window"
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
    }

    Settings {
        id: input_settings
        category: "Input"
        property alias cam_id: cameraSelector.currentValue
        property alias mirror: cameraMirrorCheckbox.checked
        property alias smartCrop: cameraSmartCropCheckbox.checked
        property alias vcam: cameraVCamCheckbox.checked
    }

    AsyncCameraCapture {
        id: cam
        deviceId: QtMultimedia.defaultCamera.deviceId
        mirror: input_settings.mirror
        smartCrop: input_settings.smartCrop
    }

    InferenceManager {
        id: manager
        camera: cam
    }

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
                    title: "Settings"

                    ColumnLayout {
                        anchors.fill: parent
                        visible: true

                        Label {
                            text: "Choose your cam:"
                        }
                        ComboBox {
                            id: cameraSelector
                            Layout.fillWidth: true
                            width: 250
                            Layout.maximumWidth: 250
                            Layout.preferredWidth: 250
                            Layout.minimumWidth: 250

                            model: QtMultimedia.availableCameras
                            valueRole: 'deviceId'
                            textRole: 'displayName'

                            onActivated: cam.deviceId = currentValue
                        }

                        Switch {
                            id: cameraMirrorCheckbox
                            Layout.fillWidth: true
                            text: "Flip video preview"

                            onClicked: cam.mirror = checked
                        }

                        Switch {
                            id: cameraSmartCropCheckbox
                            Layout.fillWidth: true
                            text: "Smart crop"

                            onClicked: cam.smartCrop = checked
                        }

                        Switch {
                            id: cameraVCamCheckbox
                            Layout.fillWidth: true
                            text: "Virtual cam"
                        }
                        Label {
                            width: 260
                            Layout.maximumWidth: 260
                            Layout.preferredWidth: 260
                            Layout.minimumWidth: 260
                            text: "You can use Avatarify as a virtual cam for Skype/Zoom calls"
                            wrapMode: Text.WordWrap
                        }
                    }
                }

                GroupBox {
                    id: previewGroup
                    title: "Preview"
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent

                        VideoOutput {
                            source: cam
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
            title: "Choose your avatar:"

            Settings {
                category: "Input"
//                property alias avatar: avatarSelector.currentValue
            }

            ColumnLayout {
                anchors.fill: parent

                ListView {
                    id: avatarSelector
                    Layout.fillWidth: true
                    height: 154
                    orientation: ListView.Horizontal
                    flickableDirection: Flickable.HorizontalFlick
                    clip: true

                    model: FolderListModel {
                        folder: "file:///Users/vlivashkin/.avatarify/avatars"
                        nameFilters: ['*.jpg', '*.jpeg', '*.png']
                    }
                    delegate: Component {
                        Image {
                            height: 144
                            width: 144
                            source: "file:///Users/vlivashkin/.avatarify/avatars/" + fileName
                        }
                    }
                }
            }
        }
    }
}
