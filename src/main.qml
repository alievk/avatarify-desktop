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
        property alias avatar: avatarSelector.currentIndex
    }

    AsyncCameraCapture {
        id: cam
        deviceId: input_settings.cam_id
        smartCrop: input_settings.smartCrop
    }

    InferenceManager {
        id: manager
        camera: cam
        mirror: input_settings.mirror
        virtualCamera: input_settings.vcam
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
                        }

                        Switch {
                            id: cameraMirrorCheckbox
                            Layout.fillWidth: true
                            text: "Flip video preview"
                        }

                        Switch {
                            id: cameraSmartCropCheckbox
                            Layout.fillWidth: true
                            text: "Smart crop"
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
                            source: manager
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

            ColumnLayout {
                anchors.fill: parent

                ListView {
                    id: avatarSelector
                    Layout.fillWidth: true
                    height: 144
                    orientation: ListView.Horizontal
                    flickableDirection: Flickable.HorizontalFlick
                    clip: true
                    highlightFollowsCurrentItem: true
                    focus: true

                    model: FolderListModel {
                        folder: "file:///Users/vlivashkin/.avatarify/avatars"
                        nameFilters: ['*.jpg', '*.jpeg', '*.png']
                    }
                    delegate: Component {
                        Item {
                            width: 144
                            height: 144
                            Image {
                                Layout.margins: 5
                                height: 134
                                width: 134
                                fillMode: Image.PreserveAspectCrop
                                source: "file:///Users/vlivashkin/.avatarify/avatars/" + fileName
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: avatarSelector.currentIndex = index
                            }
                        }
                    }
                    highlight: Rectangle {
                        color: "lightsteelblue"
                        radius: 5
                    }
                }
            }
        }
    }
}
