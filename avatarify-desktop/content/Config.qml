import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11

Item {
    id: config
    width: 270
    height: 640

    ColumnLayout {
        id: configLayout
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

        GroupBox {
            id: inputGroup
            Layout.fillWidth: true
            enabled: true
            title: qsTr("Driving input")

            ColumnLayout {
                id: inputGroupLayout
                anchors.fill: parent

                ComboBox {
                    id: cameraSelector
                    Layout.fillWidth: true
                }

                CheckBox {
                    id: cameraMirrorCheckbox
                    Layout.fillWidth: true
                    text: qsTr("Mirror")
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

            ColumnLayout {
                id: avatarGroupLayout
                anchors.fill: parent

                ComboBox {
                    id: avatarSelector
                    Layout.fillWidth: true
                }
            }
        }

        GroupBox {
            id: modelGroup
            Layout.fillWidth: true
            title: qsTr("Model")

            ColumnLayout {
                id: modelGroupLayout
                anchors.fill: parent

                ComboBox {
                    id: modelSelector
                    Layout.fillWidth: true
                }
            }
        }

        GroupBox {
            id: outputGroup
            Layout.fillWidth: true
            title: qsTr("Output")

            ColumnLayout {
                id: outputGroupLayout
                anchors.fill: parent

                CheckBox {
                    id: outputPreviewCheckbox
                    Layout.fillWidth: true
                    text: qsTr("Stream to Preview Crop")
                }

                CheckBox {
                    id: outputVcamCheckbox
                    Layout.fillWidth: true
                    text: qsTr("Virtual Camera")
                }
            }
        }
    }
}
