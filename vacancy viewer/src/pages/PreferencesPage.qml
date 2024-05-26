import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Qt5Compat.GraphicalEffects

Page {
    Rectangle {
        anchors.fill: parent
        ShaderEffect {
            anchors.fill: parent
            fragmentShader: "shader.frag.qsb"
            property real time: window.globalTime
            property real widthX: parent.width
            property real heightY: parent.height
        }
        Rectangle {
            id: prefRect
            anchors.centerIn: parent
            width: parent.width * 0.85
            height: parent.height * 0.85
            border.width: 1
            border.color: "#e1011e"
            radius: 10
            Flickable {
                anchors.fill: parent
                anchors.margins: 50
                contentHeight: contentColumn.height
                contentWidth: contentColumn.width
                boundsBehavior: Flickable.StopAtBounds
                clip: true
                ColumnLayout {
                    id: contentColumn
                    width: prefRect.width - 100
                    spacing: 30
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "Просмотренные вакансии"
                        font.pointSize: 22
                        font.bold: Font.Bold
                    }
                    Text {
                        id: viewedVacCount
                        Layout.alignment: Qt.AlignHCenter
                        font.pointSize: 16
                        Component.onCompleted: {
                            viewedVacCount.text = backend.getViewedVacsCount();
                        }
                    }
                    Button{
                        Layout.alignment: Qt.AlignHCenter
                        text: "Очистить"
                        highlighted: true
                        onClicked: {
                            clearViewsConfirmation.open();
                        }
                    }
                }
            }
        }
    }
    Dialog {
        id: clearViewsConfirmation
        height: 200
        width: 300
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        ColumnLayout {
            width: parent.width
            height: parent.height
            Text {
                width: 150
                height: 50
                text: "Удалить все просмотры?"
                font.pointSize: 14
                font.bold: Font.Bold
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                wrapMode: Text.WordWrap
            }
            RowLayout
            {
                Button {
                    width: 55
                    height: 35
                    highlighted: true
                    Layout.fillWidth: true
                    text: "Нет"
                    onClicked: {
                        clearViewsConfirmation.close();
                    }
                }
                Button {
                    width: 55
                    height: 35
                    highlighted: true
                    Layout.fillWidth: true
                    text: "Да"
                    onClicked: {
                        backend.clearAllViews();
                        viewedVacCount.text = "0";
                        clearViewsConfirmation.close();
                    }
                }
            }
        }
    }
}
