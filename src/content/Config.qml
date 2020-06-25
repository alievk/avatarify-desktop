import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11
import QtMultimedia 5.15
import Qt.labs.settings 1.0
import Qt.labs.folderlistmodel 2.15

Item {
    id: config
    width: 270
    height: 550

    ColumnLayout {
        id: configLayout
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

        GroupBox {
            Layout.fillWidth: true
            title: qsTr("Settings")

            Settings {
                category: "Input"
                property alias cam_id: cameraSelector.currentValue
                property alias mirror: cameraMirrorCheckbox.checked
                property alias smartCrop: cameraSmartCropCheckbox.checked
            }

            ColumnLayout {
                anchors.fill: parent

                Label {
                    text: "Choose your camera:"
                }
                ComboBox {
                    id: cameraSelector
                    Layout.fillWidth: true

                    model: QtMultimedia.availableCameras
                    textRole: 'displayName'
                }

                CheckBox {
                    id: cameraMirrorCheckbox
                    Layout.fillWidth: true
                    text: qsTr("Mirror Preview")
                }

                CheckBox {
                    id: cameraSmartCropCheckbox
                    Layout.fillWidth: true
                    text: qsTr("Smart Crop")
                }
            }
        }

        GroupBox {
            id: avatarGroup
            Layout.fillWidth: true
            title: qsTr("Avatar")

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
