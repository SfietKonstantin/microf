import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import org.sfietkonstantin.microf 1.0

Item {
    id: container
    property bool busy: socialItem.status === SocialNetworkStatus.Busy || socialModel.status === SocialNetworkStatus.Busy
    property bool canQuery: !busy && authHelper.accessToken !== ""
    property int type: RequestHelperModel.Invalid
    property var request: null
    property var builder: null
    property string rawData
    anchors.fill: parent
    onTypeChanged: {
        switch (container.type) {
        case RequestHelperModel.Object:
            rawData = itemBuilder.rawData
            break
        case RequestHelperModel.Model:
            rawData = container.model ? container.model.rawData : ""
            break
        default:
            break
        }
    }

    SocialContentItem {
        id: socialItem
        socialNetwork: facebook
        request: container.request
        builder: FacebookItemBuilder {
            id: itemBuilder
            includeRawData: true
        }
        onFinished: {
            container.rawData = itemBuilder.rawData
            if (!ok) {
                errorLabel.text = socialItem.errorMessage
            }
        }
    }

    SocialContentModel {
        id: socialModel
        socialNetwork: facebook
        request: container.request
        builder: container.builder
        onFinished: {
            container.rawData = container.builder ? container.builder.rawData : ""
            if (!ok) {
                errorLabel.text = socialModel.errorMessage
            }
        }
    }

    RequestHelperModel {
        id: requestModel
    }

    BuildersHelperModel {
        id: buildersModel
    }

    IntValidator {
        id: intValidator
        bottom: 0
    }

    DoubleValidator {
        id: doubleValidator
        bottom: 0
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

                        Column {
                            spacing: 3
                            anchors.left: parent.left; anchors.right: parent.right
                            ComboBox {
                                anchors.left: parent.left; anchors.right: parent.right
                                id: typeField
                                function setRequest() {
                                    container.request = requestModel.request(currentIndex)
                                    container.type = requestModel.type(currentIndex)
                                }
                                textRole: "text"
                                enabled: !container.busy
                                Layout.fillWidth: true
                                currentIndex: 0
                                onCurrentIndexChanged: setRequest()
                                Component.onCompleted: setRequest()
                                model: requestModel
                            }

                            MetaEditor {
                                object: container.request
                            }

                            Label { text: "Properties to select" }
                            TextArea {
                                anchors.left: parent.left; anchors.right: parent.right
                                onTextChanged: {
                                    Helper.setBuilderProperties(text, itemBuilder, container.builder)
                                }
                            }
                        }
                    }

                    GroupBox {
                        title: "Builder"
                        visible: container.type == RequestHelperModel.Model
                        anchors.left: parent.left; anchors.right: parent.right

                        Column {
                            spacing: 3
                            anchors.left: parent.left; anchors.right: parent.right
                            ComboBox {
                                anchors.left: parent.left; anchors.right: parent.right
                                id: builderField
                                function setRequest() {
                                    container.builder = buildersModel.builder(currentIndex)
                                }
                                textRole: "text"
                                enabled: !container.busy
                                Layout.fillWidth: true
                                currentIndex: 0
                                onCurrentIndexChanged: setRequest()
                                Component.onCompleted: setRequest()
                                model: buildersModel
                            }

                            MetaEditor {
                                object: container.builder
                            }
                        }
                    }

                    Column {
                        id: buttons
                        anchors.left: parent.left; anchors.right: parent.right
                        spacing: 3

                        Button {
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canQuery
                            text: "Request"
                            onClicked: {
                                container.rawData = ""
                                switch (container.type) {
                                case RequestHelperModel.Object:
                                    socialItem.load()
                                    break
                                case RequestHelperModel.Model:
                                    socialModel.load()
                                    break
                                default:
                                    break
                                }
                            }
                        }
                        Button {
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canQuery && type === RequestHelperModel.Model && socialModel.hasNext
                            text: "Request next"
                            onClicked: {
                                container.rawData = ""
                                socialModel.loadNext()
                            }
                        }
                        Button {
                            anchors.left: parent.left; anchors.right: parent.right
                            enabled: container.canQuery && type === RequestHelperModel.Model && socialModel.hasPrevious
                            text: "Request previous"
                            onClicked: {
                                container.rawData = ""
                                socialModel.loadPrevious()
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

        Item {
            Layout.minimumWidth: 200

            TabView {
                anchors.fill: parent
                anchors.margins: 12

                Tab {
                    title: "Display"
                    clip: true
                    Item {
                        ListView {
                            id: modelView
                            visible: container.type === RequestHelperModel.Model
                            anchors.top: parent.top; anchors.bottom: parent.bottom; anchors.margins: 12
                            width: parent.width
                            model: socialModel
                            spacing: 8
                            delegate: Display {
                                width: modelView.width
                                object: model.object
                            }
                        }

                        Flickable {
                            id: itemView
                            visible: container.type === RequestHelperModel.Object
                            anchors.top: parent.top; anchors.bottom: parent.bottom; anchors.margins: 12
                            width: parent.width
                            contentHeight: itemDisplay.height

                            Display {
                                id: itemDisplay
                                width: itemView.width
                                object: socialItem.object
                            }
                        }
                    }
                }

                Tab {
                    title: "Raw data"
                    clip: true

                    TextArea {
                        anchors.fill: parent
                        anchors.margins: 12
                        readOnly: true
                        text: container.rawData
                    }
                }
            }
        }
    }
}

