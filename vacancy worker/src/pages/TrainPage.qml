import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Qt5Compat.GraphicalEffects
import QtQuick.Dialogs

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
            anchors.centerIn: parent
            width: parent.width * 0.85
            height: parent.height * 0.85
            border.width: 1
            border.color: "#ff0000"
            radius: 10
            Item {
                id: itemPref
                anchors.fill: parent
                anchors.margins: 10
                Flickable {
                    anchors.fill: parent
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds
                    contentHeight: columnPref.height
                    contentWidth: columnPref.width
                    ColumnLayout {
                        id: columnPref
                        width: itemPref.width
                        spacing: 10
                        ColumnLayout {
                            Layout.alignment: Qt.AlignHCenter
                            Text {
                                text: "Количество эпох"
                                font.pointSize: 16
                                font.bold: Font.Bold
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text: sliderEpochs.value
                                font.pointSize: 14
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Slider {
                                id: sliderEpochs
                                from: 1
                                to: 10
                                value: 5
                                stepSize: 1
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                        ColumnLayout {
                            Layout.alignment: Qt.AlignHCenter
                            Text {
                                text: "Количество потоков (батчей)"
                                font.pointSize: 16
                                font.bold: Font.Bold
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text: sliderBatches.value
                                font.pointSize: 14
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Slider {
                                id: sliderBatches
                                from: 1
                                to: 32
                                value: 8
                                stepSize: 1
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                        ColumnLayout {
                            Layout.alignment: Qt.AlignHCenter
                            Text {
                                text: "Путь для сохранения"
                                font.pointSize: 16
                                font.bold: Font.Bold
                                Layout.alignment: Qt.AlignHCenter
                            }
                            RowLayout {
                                Layout.alignment: Qt.AlignHCenter
                                TextField {
                                    id: textfieldSavePath
                                    text: "trained/"
                                    Layout.preferredWidth: 200
                                }
                                Image {
                                    source: "qrc:/icons/search.svg"
                                    sourceSize.height: 30
                                    sourceSize.width: 30
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            filedialogSavePath.open()
                                        }
                                    }
                                }
                            }
                            FileDialog {
                                id: filedialogSavePath
                                fileMode: FileDialog.SaveFile
                                onAccepted: {
                                    textfieldSavePath.text = String(filedialogSavePath.currentFolder).substring(8);
                                }
                            }
                        }
                        ColumnLayout {
                            Layout.alignment: Qt.AlignHCenter
                            CheckBox {
                                id: checkLoadModel
                                text: "Выбрать обученную модель"
                                font.pointSize: 14
                                font.bold: Font.Bold
                                checked: false
                                Layout.alignment: Qt.AlignHCenter
                                onCheckedChanged: {
                                    if(checkLoadModel.checked)
                                    {
                                        columnLoadModel.visible = true
                                    }
                                    else
                                    {
                                        columnLoadModel.visible = false;
                                    }
                                }
                            }
                            ColumnLayout {
                                id: columnLoadModel
                                visible: false
                                Layout.alignment: Qt.AlignHCenter
                                RowLayout {
                                    Layout.alignment: Qt.AlignHCenter
                                    TextField {
                                        id: textfieldModelPath
                                        text: ""
                                        Layout.preferredWidth: 200
                                    }
                                    Image {
                                        source: "qrc:/icons/search.svg"
                                        sourceSize.height: 30
                                        sourceSize.width: 30
                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: {
                                                filedialogLoadModel.open()
                                            }
                                        }
                                    }
                                }
                                FileDialog {
                                    id: filedialogLoadModel
                                    onAccepted: {
                                        textfieldModelPath.text = String(filedialogLoadModel.currentFile).substring(8);
                                    }
                                }
                            }
                        }
                        Button {
                            id: btnTrain
                            text: "Обучить"
                            enabled: !backend.isTrainExecute
                            highlighted: true
                            Layout.alignment: Qt.AlignHCenter
                            onClicked: {
                                dialogTrain.open();
                            }
                        }
                        RowLayout {
                            id: rowTrain
                            visible: backend.isTrainExecute
                            Layout.alignment: Qt.AlignHCenter
                            BusyIndicator {}
                            Text {
                                id: textTrain
                                text: "Выполняется обучение"
                                font.pointSize: 16
                            }
                        }
                    }
                }
            }
        }
    }
    Dialog {
        id: dialogTrain
        property var rolesList: []
        width: 300
        height: 400
        x: (parent.width - dialogTrain.width)/2
        y: (parent.height - dialogTrain.height)/2
        ColumnLayout {
            id: columnRoles
            anchors.fill: parent
            ListModel{
                id: modelRoles
                Component.onCompleted: {
                    modelRoles.append(backend.getDatasetRoles())
                }
            }
            ListView {
                id: viewRoles
                clip: true
                model: modelRoles
                boundsBehavior: ListView.StopAtBounds
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 10
                delegate: ItemDelegate {
                    width: columnRoles.width
                    height: 100
                    clip: true
                    background: Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                    }
                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        CheckBox {
                            id: checkRole
                            onCheckedChanged: {
                                if(checkRole.checked)
                                {
                                    dialogTrain.rolesList.push(model.roleId);
                                }
                                else
                                {
                                    dialogTrain.rolesList.splice(dialogTrain.rolesList.indexOf(model.roleId), 1);
                                }
                            }
                        }
                        Text {
                            font.pointSize: 14
                            text: model.roleName
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                        }
                        Text {
                            font.pointSize: 14
                            text: model.roleCount
                        }
                    }
                }
            }
            RowLayout {
                Layout.fillWidth: true
                Button {
                    text: "Отмена"
                    highlighted: true
                    Layout.fillWidth: true
                    onClicked: {
                        dialogTrain.close();
                    }
                }
                Button {
                    text: "Принять"
                    highlighted: true
                    Layout.fillWidth: true
                    onClicked: {
                        if(dialogTrain.rolesList.length !== 0)
                        {
                            dialogTrain.close();
                            btnTrain.enabled = false;
                            rowTrain.visible = true;
                            var modelPath = "";
                            if(checkLoadModel.checked)
                            {
                                modelPath = textfieldModelPath.text;
                            }
                            backend.startTrain(dialogTrain.rolesList, modelPath, textfieldSavePath.text, sliderBatches.value, sliderEpochs.value);
                            dialogTrain.rolesList = [];
                        }
                    }
                }
            }
        }
    }
}
