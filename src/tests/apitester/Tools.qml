import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

Item {
    id: container

    TabView {
        anchors.fill: parent
        anchors.margins: 12
        Tab {
            title: "Url query parser"
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 12
                spacing: 3

                TextField {
                    placeholderText: "Url query"
                    Layout.fillWidth: true
                    onTextChanged: urlQueryResult.text = authHelper.parseUrlQuery(text)
                }
                TextArea {
                    id: urlQueryResult
                    textFormat: Text.RichText
                    readOnly: true
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
        Tab {
            title: "JSON format"
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 12
                spacing: 3

                TextField {
                    placeholderText: "JSON"
                    Layout.fillWidth: true
                    onTextChanged: jsonResult.text = authHelper.formatJson(text)
                }
                TextArea {
                    id: jsonResult
                    readOnly: true
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
        Tab {
            title: "Base64"
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 12
                spacing: 3

                TextField {
                    placeholderText: "Base64"
                    Layout.fillWidth: true
                    onTextChanged: base64Result.text = authHelper.fromBase64(text)
                }
                TextArea {
                    id: base64Result
                    readOnly: true
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }
}
