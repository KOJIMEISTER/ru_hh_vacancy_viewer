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
            id: profileRect
            anchors.centerIn: parent
            width: parent.width * 0.85
            height: parent.height * 0.85
            border.width: 1
            border.color: "#ff0000"
            radius: 10
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 50
                spacing: 10
                ListModel {
                    id: profilesModel
                    Component.onCompleted: {
                        profilesModel.append(backend.getAllProfiles());
                    }
                }
                ListView {
                    id: profilesView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 10
                    clip: true
                    currentIndex: -1
                    boundsBehavior: ListView.StopAtBounds
                    model: profilesModel
                    delegate: ItemDelegate {
                        width: profilesView.width
                        height: 100
                        background: Rectangle {
                          anchors.fill: parent
                          color: "transparent"
                        }
                        Rectangle {
                            anchors.fill: parent
                            radius: 10
                            border.width: 1
                            border.color: "#e1011e"
                            clip: true
                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 10
                                anchors.rightMargin: 10
                                Text {
                                    text: model.name
                                    font.pointSize: 14
                                    Layout.fillWidth: true
                                    Layout.alignment: Qt.AlignVCenter
                                    wrapMode: Text.Wrap
                                }
                                Text {
                                    text: model.current ? "Текущий" : ""
                                    font.pointSize: 14
                                    Layout.fillWidth: true
                                    Layout.alignment: Qt.AlignVCenter
                                    wrapMode: Text.Wrap
                                }
                                Button {
                                    text: "Выбрать"
                                    visible: model.current ? false : true
                                    highlighted: true
                                    Layout.alignment: Qt.AlignVCenter
                                    onClicked: {
                                        backend.setCurrentProfile(model.name);
                                        profilesModel.clear();
                                        profilesModel.append(backend.getAllProfiles());
                                    }
                                }
                                Button {
                                    text: "Удалить"
                                    visible: model.name === "Базовый" ? false : true
                                    highlighted: true
                                    Layout.alignment: Qt.AlignVCenter
                                    onClicked: {
                                        profilesView.currentIndex = model.index
                                        dialogProfileDelete.open();
                                    }
                                }
                            }
                        }
                    }
                }
                Button {
                    Layout.fillWidth: true
                    text: "Добавить"
                    highlighted: true
                    onClicked: {
                        dialogProfileAdd.open();
                    }
                }
            }
        }
    }
    Dialog {
        id: dialogProfileAdd
        width: 300
        height: 300
        x: (parent.width - width)/2
        y: (parent.height - height)/2
        Pane {
            anchors.fill: parent
            ColumnLayout
            {
                anchors.fill: parent
                TextField {
                    id: profileName
                    placeholderText: "Имя профиля"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                Button {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                    highlighted: true
                    text: "Отмена"
                    onClicked: {
                        dialogProfileAdd.close();
                    }
                }
                Button {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                    highlighted: true
                    text: "Добавить"
                    onClicked: {
                        if(profileName.text !== "")
                        {
                            backend.addProfile(profileName.text);
                            profilesModel.clear();
                            profilesModel.append(backend.getAllProfiles());
                        }
                        dialogProfileAdd.close();
                    }
                }
            }
        }
    }
    Dialog {
        id: dialogProfileDelete
        width: 300
        height: 300
        x: (parent.width - width)/2
        y: (parent.height - height)/2
        Pane {
            anchors.fill: parent
            ColumnLayout {
                anchors.fill: parent
                Text {
                    text: "Удалить профиль " + ((profilesView.currentIndex !== -1) ? profilesModel.get(profilesView.currentIndex).name : "") + "?"
                    font.pointSize: 16
                    font.bold: Font.Bold
                    Layout.fillWidth: true
                    wrapMode: Text.Wrap
                }
                RowLayout {
                    Layout.fillWidth: true
                    Button {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter
                        text: "Нет"
                        highlighted: true
                        onClicked: {
                            dialogProfileDelete.close();
                        }
                    }
                    Button {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter
                        text: "Да"
                        highlighted: true
                        onClicked: {
                            var index = profilesView.currentIndex;
                            profilesView.currentIndex = -1;
                            backend.deleteProfile(profilesModel.get(index).name);
                            profilesModel.clear();
                            profilesModel.append(backend.getAllProfiles());
                            dialogProfileDelete.close();
                        }
                    }
                }
            }
        }
    }
}
