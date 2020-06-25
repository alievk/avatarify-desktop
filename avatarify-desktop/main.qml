import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11

import "content"

Window {
    visible: true
    width: 814
    height: 500

    RowLayout {
        id: globalLayout
        anchors.fill: parent

        Config {
            id: config
            Layout.fillHeight: true
        }

        Preview {
            id: sourcePreview
            Layout.fillHeight: true
        }

        Preview {
            id: resultPreview
            Layout.fillHeight: true
        }
    }
}
