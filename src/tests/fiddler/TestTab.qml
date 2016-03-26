import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import org.sfietkonstantin.microf 1.0

Item {
    id: container

    JsonTreeModel {
        id: jsonModel
    }

    ThreadsTester {
        id: threadsTester
        facebook: fb
        model: jsonModel
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        ScrollView {
            id: buttonsView
            width: 400
            Layout.minimumWidth: 300

            Flickable {
                width: parent.width
                contentHeight: testContent.height + testContent.anchors.margins

                Column {
                    id: testContent
                    anchors.left: parent.left; anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 12
                    spacing: 8

                    GroupBox {
                        title: "Threads"
                        anchors.left: parent.left; anchors.right: parent.right

                        ColumnLayout {
                            spacing: 3
                            anchors.fill: parent
                            Button {
                                Layout.fillWidth: true
                                text: "Query threads"
                                onClicked: threadsTester.performQuery()
                            }
                        }
                    }
                }
            }
        }

        TreeView {
            anchors.top: parent.top; anchors.bottom: parent.bottom
            model: jsonModel
            Layout.minimumWidth: 200

            TableViewColumn {
                title: "Key"
                role: "key"
                width: 300
            }
            TableViewColumn {
                title: "Label"
                role: "label"
                width: 100
            }
        }
    }
}
