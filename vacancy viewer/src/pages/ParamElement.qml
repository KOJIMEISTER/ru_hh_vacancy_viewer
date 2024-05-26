import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Qt5Compat.GraphicalEffects

Rectangle {
    property alias text : param.text
    property alias textColor: param.color
    radius: 10
    width: paramMetirc.tightBoundingRect.width + 20
    height: 20
    color: "#dcdcdc"
    Text {
        id: param
        anchors.centerIn: parent
        font.pointSize: 11
    }
    TextMetrics {
        id: paramMetirc
        font: param.font
        text: param.text
    }
}
