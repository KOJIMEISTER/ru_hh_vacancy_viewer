import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Qt5Compat.GraphicalEffects

Page {
    id: datasetPage
    property string roleId: ""
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
                id: columnDataset
                anchors.fill: parent
                anchors.margins: 20
                spacing: 10
                Text {
                    text: "Специализация"
                    font.pointSize: 16
                    font.bold: Font.Bold
                    Layout.alignment: Qt.AlignHCenter
                }
                Text {
                    id: textCurRole
                    text: "не выбрано"
                    font.pointSize: 16
                    Layout.alignment: Qt.AlignHCenter
                }
                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    height: 50
                    spacing: 10
                    Button {
                        highlighted: true
                        text: "Выбрать"
                        onClicked: {
                            dialogAddRole.open();
                        }
                    }
                    Button {
                        highlighted: true
                        text: "Убрать"
                        visible: roleId === "" ? false : true
                        onClicked: {
                            textCurRole.text = "не выбрано"
                            datasetPage.roleId = "";
                        }
                    }
                    Button {
                        text: "Начать разметку"
                        highlighted: true
                        visible: roleId === "" ? 0.0 : 1.0
                        onClicked:
                        {
                            backend.clearHistory();
                            stackView.push("qrc:/pages/MarkupPage.qml")
                        }
                    }
                }
                Text {
                    text: "Направления"
                    font.pointSize: 16
                    font.bold: Font.Bold
                    Layout.alignment: Qt.AlignHCenter
                }
                Button {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Показать"
                    highlighted: true
                    onClicked: {
                        dialogRoleDirs.open()
                    }
                }
                Text {
                    text: "Специализации датасета"
                    font.pointSize: 16
                    font.bold: Font.Bold
                    Layout.alignment: Qt.AlignHCenter
                }
                RoundButton {
                    Layout.alignment: Qt.AlignHCenter
                    highlighted: true
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/icons/refresh-cw-white.svg"
                        width: parent.height - 20
                        height: parent.height - 20
                    }
                    onClicked: {
                        modelDataset.clear();
                        modelDataset.append(backend.getDatasetRoles());
                        modelRoleDirs.clear();
                        modelRoleDirs.append(backend.getAllRoleDirs());
                    }
                }
                ListModel {
                    id: modelDataset
                    Component.onCompleted: {
                        modelDataset.append(backend.getDatasetRoles());
                    }
                }
                ListView{
                    id: viewDataset
                    model: modelDataset
                    clip: true
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    cacheBuffer: 0
                    currentIndex: -1
                    spacing: 10
                    boundsBehavior: ListView.StopAtBounds
                    delegate: ItemDelegate {
                        width: columnDataset.width
                        height: 50
                        background: Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                        }
                        Rectangle {
                            anchors.fill: parent
                            radius: 10
                            border.color: "#e1011e"
                            border.width: 1
                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 10
                                anchors.rightMargin: 10
                                Text {
                                    id: textDatasetRole
                                    text: model.roleName
                                    font.pointSize: 16
                                    font.bold: Font.Bold
                                    Layout.alignment: Qt.AlignLeft
                                }
                                Text {
                                    id: textDatasetCount
                                    text: "(" + model.roleCount + ")"
                                    font.pointSize: 16
                                    font.bold: Font.Bold
                                    Layout.fillWidth: true
                                }
                                RoundButton {
                                    highlighted: true
                                    Image {
                                        anchors.centerIn: parent
                                        source: "qrc:/icons/x.svg"
                                        width: parent.height - 10
                                        height: parent.height - 10
                                    }
                                    onClicked: {
                                        viewDataset.currentIndex = index;
                                        dialogConfirmDeleteDatasetRole.open();
                                    }
                                    Layout.alignment: Qt.AlignRight
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Dialog {
        id: dialogAddRole
        width: 300
        height: 250
        x: (parent.width - width)/2
        y: (parent.height - height)/2
        ColumnLayout {
            anchors.fill: parent
            spacing: 15
            ListView {
                clip: true
                Layout.fillHeight: true
                Layout.fillWidth: true
                anchors.bottom: parent.Bottom
                width: parent.width
                ScrollIndicator.vertical: ScrollIndicator {
                    active: true
                }
                model: ListModel {
                    id: foundRolesModel
                    Component.onCompleted: {
                        foundRolesModel.clear();
                        foundRolesModel.append(backend.getAllRoles());
                    }
                }
                delegate: ItemDelegate {
                    width: parent.width - 10
                    height: 30
                    clip: true
                    Row {
                        anchors.fill: parent
                        Rectangle {
                            width: parent.width * 0.75
                            height: parent.height
                            Text{
                                text: model.name
                                font.pointSize: 12
                                anchors.left: parent.left
                            }
                        }
                        Rectangle {
                            width: parent.width * 0.25
                            height: parent.height
                            clip: true
                            Text{
                                text: model.count
                                anchors.right: parent.right
                                font.pointSize: 12
                            }
                        }
                    }
                    onClicked: {
                        textCurRole.text = model.name + "(id:" + model.roleId + ")";
                        datasetPage.roleId = model.roleId;
                    }
                }
            }
            RoundButton {
                Layout.alignment: Qt.AlignHCenter
                highlighted: true
                onClicked: {
                    dialogAddRole.close();
                }
                Image {
                    anchors.fill: parent
                    anchors.margins: 10
                    source: "qrc:/icons/x.svg"
                }
            }
        }
    }
    Dialog {
        id: dialogConfirmDeleteDatasetRole
        height: 200
        width: 350
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        ColumnLayout {
            id: columnDeleteConfirm
            anchors.fill: parent
            Text {
                Layout.fillWidth: true
                text: (viewDataset.currentIndex !== -1) ? "Удалить набор данных: " + modelDataset.get(viewDataset.currentIndex).roleName + "?" : ""
                font.pointSize: 14
                wrapMode: Text.WordWrap
                font.bold: Font.Bold
            }
            RowLayout {
                Layout.fillWidth: true
                Button {
                    Layout.fillWidth: true
                    highlighted: true
                    text: "Нет"
                    onClicked: {
                        dialogConfirmDeleteDatasetRole.close();
                    }
                }
                Button {
                    Layout.fillWidth: true
                    highlighted: true
                    text: "Да"
                    onClicked: {
                        backend.deleteDatasetRole(modelDataset.get(viewDataset.currentIndex).roleId);
                        modelDataset.remove(viewDataset.currentIndex);
                        viewDataset.currentIndex = -1;
                        dialogConfirmDeleteDatasetRole.close();
                    }
                }
            }
        }
    }
    Dialog {
        id: dialogRoleDirs
        height: 350
        width: 350
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        ListModel {
            id: modelRoleDirs
            Component.onCompleted: {
                modelRoleDirs.append(backend.getAllRoleDirs());
            }
        }
        ColumnLayout {
            id: columnDirs
            anchors.fill: parent
            ListView {
                id: viewRoleDirs
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true
                currentIndex: -1
                boundsBehavior: ListView.StopAtBounds
                model: modelRoleDirs
                delegate: ItemDelegate {
                    width: columnDirs.width
                    height: 100
                    clip: true
                    background: Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                    }
                    RowLayout {
                        anchors.fill: parent
                        Text {
                            text: model.dirName
                            font.pointSize: 14
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                        }
                        Text {
                            text: model.count
                            font.pointSize: 14
                            Layout.alignment: Qt.AlignRight
                            wrapMode: Text.WordWrap
                        }
                        RoundButton {
                            Layout.alignment: Qt.AlignRight
                            highlighted: true
                            Image {
                                anchors.fill: parent
                                anchors.margins: 10
                                source: "qrc:/icons/x.svg"
                            }
                            onClicked: {
                                viewRoleDirs.currentIndex = index;
                                dialogConfirmRoleDelete.open();
                            }
                        }
                    }
                }
            }
            RoundButton {
                Layout.alignment: Qt.AlignHCenter
                highlighted: true
                Image {
                    anchors.fill: parent
                    anchors.margins: 10
                    source: "qrc:/icons/x.svg"
                }
                onClicked: {
                    dialogRoleDirs.close();
                }
            }
        }
    }
    Dialog {
        id: dialogConfirmRoleDelete
        height: 200
        width: 350
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        ColumnLayout {
            anchors.fill: parent
            clip: true
            Text {
                text: (viewRoleDirs.currentIndex === -1) ? "" : "Удалить направление " + modelRoleDirs.get(viewRoleDirs.currentIndex).dirName + "? Будет удалено вакансий: " + modelRoleDirs.get(viewRoleDirs.currentIndex).count
                wrapMode: Text.WordWrap
                font.pointSize: 14
                font.bold: Font.Bold
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            RowLayout {
                Button {
                    Layout.fillWidth: true
                    text: "Нет"
                    highlighted: true
                    onClicked: {
                        dialogConfirmRoleDelete.close();
                    }
                }
                Button {
                    Layout.fillWidth: true
                    text: "Да"
                    highlighted: true
                    onClicked: {
                        backend.deleteRoleDir(modelRoleDirs.get(viewRoleDirs.currentIndex).dirName);
                        modelRoleDirs.remove(viewRoleDirs.currentIndex);
                        viewRoleDirs.currentIndex = -1;
                        dialogConfirmRoleDelete.close();
                    }
                }
            }
        }
    }
}
