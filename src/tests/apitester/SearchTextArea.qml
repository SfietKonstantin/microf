import QtQuick 2.3
import QtQuick.Controls 1.2
import org.sfietkonstantin.microf 1.0

Item {
    id: container
    property alias text: textArea.text
    property alias textFormat: textArea.textFormat
    property alias readOnly: textArea.readOnly

    TextArea {
        id: textArea
        anchors.left: parent.left; anchors.right: parent.right
        anchors.top: parent.top; anchors.bottom: search.top
        readOnly: true
    }
    TextField {
        id: search
        anchors.left: parent.left; anchors.right: next.left
        anchors.bottom: parent.bottom
        placeholderText: "Search"
    }

    Button {
        id: next
        height: search.height
        text: "Next"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: searchHelper.next()
    }
    SearchHelper {
        id: searchHelper
        textDocument: textArea.textDocument
        text: search.text
        onSelectionChanged: textArea.select(begin, end)
    }
}





