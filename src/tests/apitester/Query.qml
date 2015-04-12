import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import org.sfietkonstantin.microf 1.0

Item {
    id: container
    property bool busy: model.status === SocialNetworkStatus.Busy
    property bool canQuery: !busy && authHelper.accessToken !== ""
    anchors.fill: parent

    FacebookFriendListRequest {
        id: friendsRequest
        userId: authHelper.userId
        size: 10
    }

    SocialContentModel {
        id: model
        socialNetwork: facebook
        builder: FacebookModelBuilder {}
        onFinished: {
            if (!ok) {
                errorLabel.text = model.errorString
            }
        }
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        ScrollView {
            id: infoView
            width: 300
            Layout.minimumWidth: 200

            Flickable {
                width: parent.width
                contentHeight: infoViewContent.height + infoViewContent.anchors.margins

                Column {
                    id: infoViewContent
                    anchors.left: parent.left; anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 12
                    spacing: 8

                    GroupBox {
                        title: "Request"
                        anchors.left: parent.left; anchors.right: parent.right

                        ColumnLayout {
                            spacing: 3
                            anchors.fill: parent
                            ComboBox {
                                id: typeField
                                function setRequest() {
                                    var mapping = [friendsRequest]
                                    model.request = mapping[currentIndex]
                                }
                                enabled: !container.busy
                                Layout.fillWidth: true
                                currentIndex: 0
                                onCurrentIndexChanged: setRequest()
                                Component.onCompleted: setRequest()
                                model: ListModel {
                                    id: queryModel
                                    ListElement { text: "Friends" }
                                }
                            }
                        }
                    }

                    Column {
                        anchors.left: parent.left; anchors.right: parent.right
                        spacing: 3

                        Button {
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canQuery
                            text: "Request"
                            onClicked: {
                                model.load()
                            }
                        }
                        Button {
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canQuery && model.hasNext
                            text: "Request next"
                            onClicked: {
                                model.loadNext()
                            }
                        }
                        Button {
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canQuery && model.hasPrevious
                            text: "Request previous"
                            onClicked: {
                                model.loadPrevious()
                            }
                        }
                    }

                    Label {
                        id: errorLabel
                        anchors.left: parent.left; anchors.right: parent.right
                        color: "red"
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }

        ListView {
            id: view
            Layout.minimumWidth: 200
            width: parent.width
            model: model
            delegate: Item {
                width: view.width
                height: rect.height + rect.anchors.margins

                Rectangle {
                    id: rect
                    anchors.left: parent.left; anchors.right: parent.right
                    anchors.margins: 12
                    anchors.verticalCenter: parent.verticalCenter
                    height: column.height + column.anchors.margins * 2

                    InfoHelper {
                        id: infoHelper
                        object: model.object
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
        }
    }
}

