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
            id: infoRect
            anchors.centerIn: parent
            width: parent.width * 0.85
            height: parent.height * 0.85
            border.width: 1
            border.color: "#ff0000"
            radius: 10
            Flickable {
                anchors.fill: parent
                anchors.margins: 50
                contentHeight: contentFlow.height
                contentWidth: contentFlow.width
                clip: true
                boundsBehavior: Flickable.StopAtBounds
                ColumnLayout {
                    id: contentFlow
                    width: infoRect.width - 100
                    spacing: 30
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: qsTr("Преимущества использования программы");
                        font.bold: Font.Bold
                        font.pointSize: 22
                        wrapMode: Text.Wrap
                        elide: Text.ElideMiddle
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("1) Предсказывает направления для вакансий, позволяя пользователю сэкономить время на анализе требований и обязанностей");
                        font.pointSize: 16
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("2) Ведет учет просмотренных вакансий, недопуская повтороного появления вакансии в поиске");
                        font.pointSize: 16
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("3) Ведет учет выполненных поисков, позволяя вернутся к поиску");
                        font.pointSize: 16
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("4) Ведет учет количества просмотренных и непросмотренных вакансий для поиска, что позволяет узнать о появлении новых вакансий не выполняя поиск");
                        font.pointSize: 16
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("5) Позволяет добавлять и исключать ключевые навыки в поиске с помощью интерфейса, вместо языка поисковых запросов");
                        font.pointSize: 16
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("6) Требования и обязанности в содержании вакансии переносятся в начало, чтобы пользователь не искал их по всей вакансии");
                        font.pointSize: 16
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("7) Убирает из поиска дублирующиеся вакансии, которых 15-25% от всего количества вакансий");
                        font.pointSize: 16
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("8) Убирает ограничение на глубину поиска в 2000 вакансий");
                        font.pointSize: 16
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("9) Информация о вакансиях подгружается и выгружается динамически, что снижает потребление интернета, а также экономит ресурсы процессора");
                        font.pointSize: 16
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("10) Содержание вакансии можно увидеть без переходов и возвращения на страницы");
                        font.pointSize: 16
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: qsTr("О программе");
                        font.bold: Font.Bold
                        font.pointSize: 22
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("Цель программы - ускорить просмотр вакансий с сайта HeadHunter");
                        font.pointSize: 16
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("Программа разработана в рамках выпускной квалификационной работы бакалавра направления \"Программная инженерия\"");
                        font.pointSize: 16
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: qsTr("Об авторе");
                        font.bold: Font.Bold
                        font.pointSize: 22
                        wrapMode: Text.Wrap
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("Авторство над программой \"IT Разработчик\" принадлежит Рябову Никите Алексеевичу");
                        font.pointSize: 16
                        wrapMode: Text.Wrap
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 30
                        Image {
                            source: "qrc:/icons/github.svg"
                            sourceSize.width: 30
                            sourceSize.height: 30
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    Qt.openUrlExternally("https://github.com/KOJIMEISTER");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
