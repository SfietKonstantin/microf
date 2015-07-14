import QtQuick 2.0
import org.sfietkonstantin.microf 1.0
import QtQuick.Controls 1.2

Item {
    id: container

    property alias object: infoHelper.object
    visible: infoHelper.text !== ""
    height: rect.height

    Rectangle {
        id: rect
        anchors.left: parent.left; anchors.right: parent.right
        anchors.margins: 12
        height: column.height + column.anchors.margins * 2

        InfoHelper {
            id: infoHelper
        }

        Column {
            id: column
            anchors.left: parent.left; anchors.right: parent.right
            anchors.margins: 12
            anchors.verticalCenter: parent.verticalCenter
            spacing: 8
            Label {
                id: label
                anchors.left: parent.left; anchors.right: parent.right
                anchors.margins: 8
                wrapMode: Text.WrapAnywhere
                textFormat: Text.RichText
                text: infoHelper.text
            }

            Repeater {
                anchors.left: parent.left; anchors.right: parent.right
                model: infoHelper.urls

                Image {
                    anchors.left: parent.left; anchors.right: parent.right
                    source: model.modelData
                    fillMode: Image.PreserveAspectFit
                    visible: status === Image.Ready
                }
            }
        }
    }
}
