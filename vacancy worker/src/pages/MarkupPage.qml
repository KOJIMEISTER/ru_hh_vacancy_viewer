import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Qt5Compat.GraphicalEffects

Page {
    id: markupPage
    property var data: {"vacId":"", "vacDesc":"", "vacName":"", "vacTechs":[], "vacMarked":false}
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
                RowLayout {
                    Layout.fillWidth: true
                    RoundButton {
                        highlighted: true
                        Image {
                            anchors.centerIn: parent
                            source: "qrc:/icons/chevron-left.svg"
                            width: parent.height - 10
                            height: parent.height - 10
                        }
                        onClicked: {
                            stackView.pop();
                        }
                    }
                    Switch {
                        id: switchAutoFormate
                        text: "Автоматическое структурирование"
                        checked: true
                        font.pointSize: 16
                        onCheckedChanged: {
                            if(switchAutoFormate.checked)
                            {
                                var model = markupPage.data;
                                model.vacDesc = backend.reformateText(markupPage.data.vacDesc);
                                markupPage.data = model;
                            }
                            else
                            {
                                model = markupPage.data;
                                model.vacDesc = backend.getVacancyDescription(markupPage.data.vacId);
                                markupPage.data = model;
                            }
                        }
                    }
                    RoundButton {
                        id: btnClearLabel
                        highlighted: true
                        visible: markupPage.data.vacMarked ? 1.0 : 0.0
                        Layout.alignment: Qt.AlignRight
                        Image {
                            anchors.centerIn: parent
                            source: "qrc:/icons/x.svg"
                            width: parent.height - 10
                            height: parent.height - 10
                        }
                        onClicked: {
                            backend.clearVacLabel(markupPage.data.vacId);
                            var model = markupPage.data;
                            model.vacMarked = false;
                            markupPage.data = model;
                        }
                    }
                }
                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    ColumnLayout {
                        id: columnVacData
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        spacing: 5
                        clip: true
                        Text {
                            id: textVacName
                            text: markupPage.data.vacName
                            Layout.preferredWidth: columnVacData.width
                            wrapMode: Text.WordWrap
                            font.bold: Font.Bold
                            font.pointSize: 16
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    Qt.openUrlExternally("https://hh.ru/vacancy/" + markupPage.data.vacId);
                                }
                            }
                        }
                        Flickable {
                            id: scrollDesctiption
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            contentHeight: textDescription.height
                            contentWidth: textDescription.width
                            boundsBehavior: Flickable.StopAtBounds
                            clip: true
                            Text {
                                id: textDescription
                                text: markupPage.data.vacDesc
                                font.pointSize: 14
                                width: columnVacData.width
                                wrapMode: Text.WordWrap
                            }
                        }
                    }
                    ColumnLayout {
                        id: columnInputs
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Text {
                            text: "Ключевые навыки"
                            font.pointSize: 16
                            Layout.alignment: Qt.AlignHCenter
                            Layout.preferredWidth: columnInputs.width
                        }
                        Flickable {
                            clip: true
                            Layout.fillHeight: true
                            Layout.preferredWidth: columnInputs.width
                            contentHeight: flowTechs.height
                            contentWidth: flowTechs.width
                            boundsBehavior: Flickable.StopAtBounds
                            Flow {
                                id: flowTechs
                                width: columnInputs.width
                                spacing: 5
                                Repeater {
                                    model: markupPage.data.vacTechs
                                    Rectangle {
                                        width: textMetricTechName.tightBoundingRect.width + 20
                                        height: 25
                                        radius: 10
                                        color: "#DCDCDC"
                                        Text {
                                            id: textTechName
                                            text: model.modelData
                                            anchors.centerIn: parent
                                            font.pointSize: 14
                                        }
                                        TextMetrics {
                                            id: textMetricTechName
                                            font: textTechName.font
                                            text: textTechName.text
                                        }
                                    }
                                }
                            }
                        }
                        Text {
                            text: "Добавить направление"
                            font.pointSize: 16
                            Layout.alignment: Qt.AlignHCenter
                            Layout.preferredWidth: columnInputs.width
                        }
                        TextField {
                            id: textFieldDir
                            placeholderText: "Введите название направления"
                            font.pointSize: 16
                            Layout.alignment: Qt.AlignHCenter
                            Layout.preferredWidth: columnInputs.width
                        }
                        Button {
                            text: "Добавить"
                            Layout.alignment: Qt.AlignHCenter
                            Layout.preferredWidth: columnInputs.width
                            highlighted: true
                            onClicked: {
                                if(textFieldDir.text !== "")
                                {
                                    var roleId = stackView.get(1, StackView.DontLoad).roleId;
                                    if(backend.addDir(textFieldDir.text, roleId))
                                    {
                                        var model = backend.getDirs(roleId);
                                        model.unshift("Не выбрано");
                                        comboDirs.model = model;
                                    }
                                }
                                textFieldDir.text = "";
                            }
                        }
                        Text {
                            text: "Назначить направление вакансии"
                            font.pointSize: 16
                            Layout.alignment: Qt.AlignHCenter
                            Layout.preferredWidth: columnInputs.width
                        }
                        ComboBox {
                            id: comboDirs
                            model: ["Не выбрано"]
                            Layout.alignment: Qt.AlignHCenter
                            Layout.preferredWidth: columnInputs.width
                            Component.onCompleted: {
                                var roleId = stackView.get(1, StackView.DontLoad).roleId;
                                var model = backend.getDirs(roleId);
                                model.unshift("Не выбрано");
                                comboDirs.model = model;
                            }
                        }
                        Row {
                            Layout.alignment: Qt.AlignHCenter
                            spacing: 5
                            Button {
                                text: "Назад"
                                highlighted: true
                                onClicked: {
                                    var roleId = stackView.get(1, StackView.DontLoad).roleId;
                                    backend.previousVac();
                                    markupPage.data = backend.getCurrentVac();
                                    if(switchAutoFormate.checked)
                                    {
                                        var model = markupPage.data;
                                        model.vacDesc = backend.reformateText(markupPage.data.vacDesc);
                                        markupPage.data = model;
                                    }
                                }
                            }
                            Button {
                                text: "Не показывать"
                                highlighted: true
                                onClicked: {
                                    backend.setVacSkipped(markupPage.data.vacId);
                                }
                            }
                            Button {
                                text: "Сохранить"
                                highlighted: true
                                onClicked: {
                                    if(comboDirs.currentIndex !== 0)
                                    {
                                        backend.addVacToDataset(markupPage.data.vacId, comboDirs.currentText);
                                        comboDirs.currentIndex = 0;
                                        var model = markupPage.data;
                                        model.vacMarked = true;
                                        markupPage.data = model;
                                    }
                                }
                            }
                            Button {
                                text: "Далее"
                                highlighted: true
                                onClicked: {
                                    var roleId = stackView.get(1, StackView.DontLoad).roleId;
                                    backend.nextVac(roleId);
                                    markupPage.data = backend.getCurrentVac();
                                    if(switchAutoFormate.checked)
                                    {
                                        var model = markupPage.data;
                                        model.vacDesc = backend.reformateText(markupPage.data.vacDesc);
                                        markupPage.data = model;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
