import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQuick.XmlListModel 2.0

Item {
    id: container

    ColumnLayout {
        anchors.fill: parent; anchors.margins: 12
        spacing: 3

        TextField {
            id: path
            placeholderText: "Path to Burp log file"
            Layout.fillWidth: true
            onTextChanged: info.clear()
        }

        SplitView {
            orientation: Qt.Horizontal
            Layout.fillWidth: true
            Layout.fillHeight: true

            Item {
                Layout.minimumWidth: 450

                TableView {
                    anchors.fill: parent
                    anchors.margins: 12
                    alternatingRowColors: false

                    TableViewColumn {
                        role: "index"
                        title: "#"
                        movable: false
                        width: 50
                    }

                    TableViewColumn {
                        role: "time"
                        title: "Time"
                        movable: false
                        delegate: Label {
                            color: styleData.textColor
                            elide: styleData.elideMode
                            text: authHelper.formatDate(styleData.value)
                        }
                        width: 50
                    }
                    TableViewColumn {
                        role: "url"
                        title: "Url"
                        movable: false
                        width: 300
                    }

                    model: XmlListModel {
                        source: "file://" + path.text
                        query: "/items/item"
                        roles: [
                            XmlRole {name: "time"; query: "time/string()"},
                            XmlRole {name: "url"; query: "url/string()"},
                            XmlRole {name: "path"; query: "path/string()"},
                            XmlRole {name: "request"; query: "request/string()"},
                            XmlRole {name: "reply"; query: "response/string()"}
                        ]
                    }
                    onCurrentRowChanged: {
                        var object = new Object
                        object.url = model.get(currentRow).url
                        object.request = authHelper.formatBurpRequestInfo(model.get(currentRow).request)
                        object.query = authHelper.formatBurpRequestQuery(model.get(currentRow).url)
                        object.postData = authHelper.formatBurpRequestPostData(model.get(currentRow).request)
                        object.reply = authHelper.formatBurpReply(model.get(currentRow).reply)
                        object.replyData = authHelper.formatBurpReplyData(model.get(currentRow).reply)
                        info.display(object)
                    }
                }
            }

            Item {
                id: info

                QtObject {
                    id: propertiesObject
                    property string url
                    property string request
                    property string query
                    property string postData
                    property string reply
                    property string replyData
                }

                function clear() {
                    propertiesObject.url = ""
                    propertiesObject.request = ""
                    propertiesObject.query = ""
                    propertiesObject.postData = ""
                    propertiesObject.reply = ""
                    propertiesObject.replyData = ""
                }
                function display(object) {
                    propertiesObject.url = object.url
                    propertiesObject.request = object.request
                    propertiesObject.query = object.query
                    propertiesObject.postData = object.postData
                    propertiesObject.reply = object.reply
                    propertiesObject.replyData = object.replyData
                }

                Layout.minimumWidth: 400
                TabView {
                    anchors.fill: parent
                    anchors.margins: 12
                    Tab {
                        title: "Request"

                        TabView {
                            anchors.fill: parent
                            anchors.margins: 12

                            Tab {
                                title: "Info"

                                ColumnLayout {
                                    anchors.fill: parent; anchors.margins: 12
                                    spacing: 3

                                    TextField {
                                        Layout.fillWidth: true
                                        readOnly: true
                                        text: propertiesObject.url
                                    }

                                    SearchTextArea {
                                        textFormat: Text.RichText
                                        readOnly: true
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        text: propertiesObject.request
                                    }
                                }
                            }
                            Tab {
                                title: "Query"
                                clip: true

                                SearchTextArea {
                                    anchors.fill: parent
                                    anchors.margins: 12
                                    textFormat: Text.RichText
                                    readOnly: true
                                    text: propertiesObject.query
                                }
                            }
                            Tab {
                                title: "Data"
                                clip: true

                                SearchTextArea {
                                    anchors.fill: parent
                                    anchors.margins: 12
                                    textFormat: Text.RichText
                                    readOnly: true
                                    text: propertiesObject.postData
                                }
                            }
                        }
                    }

                    Tab {
                        title: "Reply"

                        TabView {
                            anchors.fill: parent
                            anchors.margins: 12

                            Tab {
                                title: "Info"
                                clip: true

                                SearchTextArea {
                                    anchors.fill: parent
                                    anchors.margins: 12
                                    textFormat: Text.RichText
                                    readOnly: true
                                    text: propertiesObject.reply
                                }
                            }
                            Tab {
                                title: "Data"
                                clip: true

                                SearchTextArea {
                                    anchors.fill: parent
                                    anchors.margins: 12
                                    readOnly: true
                                    text: propertiesObject.replyData
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
