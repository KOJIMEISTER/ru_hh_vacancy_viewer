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
            anchors.centerIn: parent
            width: parent.width * 0.85
            height: parent.height * 0.85
            border.width: 1
            border.color: "#ff0000"
            radius: 10
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                Text {
                    text: "Всего вакансий " + backend.getVacCount();
                    font.pointSize: 32
                    font.bold: Font.Bold
                    Layout.alignment: Qt.AlignHCenter
                }
                ColumnLayout {
                    id: columnRefresh
                    Layout.alignment: Qt.AlignHCenter
                    state: backend.curState
                    states: [
                        State {
                            name: "Idle"
                            PropertyChanges { target: btnStartRefresh; visible: true; enabled: true}
                            PropertyChanges { target: btnStopRefresh; visible: false; enabled: false}
                            PropertyChanges { target: busyStopping; running: false}
                            PropertyChanges { target: rowStop; visible: false}
                            PropertyChanges { target: columnLoadStatus; visible: false}
                            PropertyChanges { target: textLoadStatus; text: ""}
                            PropertyChanges { target: busyLoadStatus; running: false}
                            PropertyChanges { target: barLoadStatus; indeterminate: true}
                            PropertyChanges { target: textLoadStatusPercents; text: ""}
                            PropertyChanges { target: busyHandleStatus; running: false}
                            PropertyChanges { target: barHandleStatus; visible: false}
                            PropertyChanges { target: textHandleStatusPercents; text: ""}
                        },
                        State {
                            name: "Init"
                            PropertyChanges { target: btnStartRefresh; visible: true; enabled: false}
                            PropertyChanges { target: btnStopRefresh; visible: true; enabled: true}
                            PropertyChanges { target: busyStopping; running: false}
                            PropertyChanges { target: rowStop; visible: false}
                            PropertyChanges { target: columnLoadStatus; visible: true}
                            PropertyChanges { target: textLoadStatus; text: "1/4 Инициализация"}
                            PropertyChanges { target: busyLoadStatus; running: false; visible: false}
                            PropertyChanges { target: barLoadStatus; indeterminate: true}
                            PropertyChanges { target: textLoadStatusPercents; text: ""}
                            PropertyChanges { target: busyHandleStatus; running: false}
                            PropertyChanges { target: barHandleStatus; visible: false}
                            PropertyChanges { target: textHandleStatusPercents; text: ""}
                        },
                        State {
                            name: "Validate"
                            PropertyChanges { target: btnStartRefresh; visible: true; enabled: false}
                            PropertyChanges { target: btnStopRefresh; visible: true; enabled: true}
                            PropertyChanges { target: busyStopping; running: false}
                            PropertyChanges { target: rowStop; visible: false}
                            PropertyChanges { target: columnLoadStatus; visible: true}
                            PropertyChanges { target: textLoadStatus; text: "2/4 Проверка вакансий на архивность"}
                            PropertyChanges { target: busyLoadStatus; running: true; visible: true}
                            PropertyChanges { target: barLoadStatus; indeterminate: false}
                            PropertyChanges { target: textLoadStatusPercents; text: Number(Math.ceil((100 * barLoadStatus.value / barLoadStatus.to))) + '%'}
                            PropertyChanges { target: busyHandleStatus; running: false}
                            PropertyChanges { target: barHandleStatus; visible: false}
                            PropertyChanges { target: textHandleStatusPercents; text: ""}
                        },
                        State {
                            name: "Load"
                            PropertyChanges { target: btnStartRefresh; visible: true; enabled: false}
                            PropertyChanges { target: btnStopRefresh; visible: true; enabled: true}
                            PropertyChanges { target: busyStopping; running: false}
                            PropertyChanges { target: rowStop; visible: false}
                            PropertyChanges { target: columnLoadStatus; visible: true}
                            PropertyChanges { target: textLoadStatus; text: "3/4 Загрузка новых вакансий"}
                            PropertyChanges { target: busyLoadStatus; running: true; visible: true}
                            PropertyChanges { target: barLoadStatus; indeterminate: false}
                            PropertyChanges { target: textLoadStatusPercents; text: Number(Math.ceil((100 * barLoadStatus.value / barLoadStatus.to))) + '%'}
                            PropertyChanges { target: busyHandleStatus; running: true}
                            PropertyChanges { target: barHandleStatus; visible: true}
                            PropertyChanges { target: textHandleStatusPercents; text: Number(Math.ceil((100 * barHandleStatus.value / barHandleStatus.to))) + '%'}
                        },
                        State {
                            name: "Hash"
                            PropertyChanges { target: btnStartRefresh; visible: true; enabled: false}
                            PropertyChanges { target: btnStopRefresh; visible: true; enabled: true}
                            PropertyChanges { target: busyStopping; running: false}
                            PropertyChanges { target: rowStop; visible: false}
                            PropertyChanges { target: columnLoadStatus; visible: true}
                            PropertyChanges { target: textLoadStatus; text: "4/4 Хеширование содержаний и выявление дубликатов"}
                            PropertyChanges { target: busyLoadStatus; running: true; visible: true}
                            PropertyChanges { target: barLoadStatus; indeterminate: true}
                            PropertyChanges { target: textLoadStatusPercents; text: ""}
                            PropertyChanges { target: busyHandleStatus; running: false}
                            PropertyChanges { target: barHandleStatus; visible: false}
                            PropertyChanges { target: textHandleStatusPercents; text: ""}
                        },
                        State {
                            name: "Stop"
                            PropertyChanges { target: btnStartRefresh; visible: true; enabled: false}
                            PropertyChanges { target: btnStopRefresh; visible: false; enabled: false}
                            PropertyChanges { target: busyStopping; running: true}
                            PropertyChanges { target: rowStop; visible: true}
                            PropertyChanges { target: columnLoadStatus; visible: false}
                            PropertyChanges { target: textLoadStatus; text: ""}
                            PropertyChanges { target: busyLoadStatus; running: false; visible: true}
                            PropertyChanges { target: barLoadStatus; indeterminate: true}
                            PropertyChanges { target: textLoadStatusPercents; text: ""}
                            PropertyChanges { target: busyHandleStatus; running: false}
                            PropertyChanges { target: barHandleStatus; visible: false}
                            PropertyChanges { target: textHandleStatusPercents; text: ""}
                        }
                    ]
                    Button {
                        id: btnStartRefresh
                        text: "Обновить"
                        highlighted: true
                        Layout.alignment: Qt.AlignHCenter
                        onClicked: {
                            dialogChooseRoles.open()
                        }
                    }
                    Button {
                        id: btnStopRefresh
                        text: "Остановить"
                        highlighted: true
                        visible: false
                        Layout.alignment: Qt.AlignHCenter
                        onClicked: {
                            backend.curState = "Stop";
                            backend.stopRefresh();
                        }
                    }
                    RowLayout{
                        id: rowStop
                        BusyIndicator {
                            id: busyStopping
                            running: false
                        }
                        Text {
                            text: "Остановка"
                            font.pointSize: 16
                            font.bold: Font.Bold
                        }
                    }
                    ColumnLayout {
                        id: columnLoadStatus
                        visible: false
                        Layout.alignment: Qt.AlignBottom
                        Text {
                            id: textLoadStatus
                            font.pointSize: 16
                            font.bold: Font.Bold
                            Layout.alignment: Qt.AlignHCenter
                        }
                        ProgressBar {
                            id: barLoadStatus
                            from: 0
                            to: backend.maxValue
                            value: backend.curValue
                            Layout.alignment: Qt.AlignHCenter
                        }
                        BusyIndicator {
                            id: busyLoadStatus
                            running: false
                            Layout.alignment: Qt.AlignHCenter
                            Text {
                                id: textLoadStatusPercents
                                font.pointSize: 14
                                font.bold: Font.Bold
                                anchors.centerIn: parent
                            }
                        }
                        ProgressBar {
                            id: barHandleStatus
                            from: 0
                            to: backend.maxValueHandle
                            value: backend.curValueHandle
                            Layout.alignment: Qt.AlignHCenter
                        }
                        BusyIndicator {
                            id: busyHandleStatus
                            running: false
                            Layout.alignment: Qt.AlignHCenter
                            Text {
                                id: textHandleStatusPercents
                                font.pointSize: 14
                                font.bold: Font.Bold
                                anchors.centerIn: parent
                            }
                        }
                    }
                }
            }
        }
    }
    Dialog {
        id: dialogChooseRoles
        width: 400
        height: 500
        x: (parent.width - width)/2
        y: (parent.height - height)/2
        ColumnLayout {
            id: columnRoles
            anchors.fill: parent
            clip: true
            CheckBox {
                checked: false
                text: "Выбрать все"
                font.pointSize: 14
                onCheckedChanged: {
                    for(var i = 0; i < modelRoles.count; ++i)
                    {
                        var role = modelRoles.get(i)
                        role.checked = checked
                        modelRoles.set(i, role)
                    }
                }
            }
            ListModel {
                id: modelRoles
                Component.onCompleted: {
                    modelRoles.append(backend.getAllRoles())
                }
            }
            ListView {
                id: viewRoles
                model: modelRoles
                clip: true
                boundsBehavior: ListView.StopAtBounds
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 10
                delegate: ItemDelegate {
                    width: columnRoles.width
                    height: 100
                    background: Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                    }
                    RowLayout {
                        anchors.fill: parent
                        CheckBox {
                            checked: model.checked
                            onCheckedChanged: {
                                model.checked = checked
                            }
                        }
                        Text {
                            text: model.name
                            font.pointSize: 14
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                        }
                        Text {
                            text: model.count
                            font.pointSize: 14
                        }
                    }
                }
            }
            RowLayout {
                Layout.fillWidth: true
                Text {
                    text: "Период дней"
                    font.pointSize: 14
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                }
                SpinBox {
                    id: spinboxDays
                    from: 1
                    to: 30
                    value: 1
                    stepSize: 1
                }
            }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Button {
                    text: "Отмена"
                    highlighted: true
                    onClicked: {
                        dialogChooseRoles.close()
                    }
                }
                Button {
                    text: "Обновить"
                    highlighted: true
                    onClicked: {
                        dialogChooseRoles.close()
                        backend.curState = "Init";
                        var rolesList = []
                        for(var i = 0; i < modelRoles.count; ++i)
                        {
                            if(modelRoles.get(i).checked)
                            {
                                rolesList.push(modelRoles.get(i).roleId);
                            }
                        }
                        backend.refreshVacs(rolesList, spinboxDays.value);
                    }
                }
            }
        }
    }
}
