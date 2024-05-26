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
            id: rectMain
            anchors.centerIn: parent
            width: parent.width * 0.85
            height: parent.height * 0.85
            border.width: 1
            border.color: "#ff0000"
            radius: 10
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 10
                ColumnLayout {
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredHeight: rectMain.height / 2
                    Layout.preferredWidth: rectMain.width / 2
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "СБОР ДАТАСЕТА"
                        wrapMode: Text.WordWrap
                        font.bold: Font.Bold
                        font.pointSize: 32
                    }
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "ОБУЧЕНИЕ МОДЕЛИ"
                        wrapMode: Text.WordWrap
                        font.bold: Font.Bold
                        font.pointSize: 32
                    }
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "ОБНОВЛЕНИЕ ВАКАНСИЙ"
                        wrapMode: Text.WordWrap
                        font.bold: Font.Bold
                        font.pointSize: 32
                    }
                }
            }
        }
    }
}
