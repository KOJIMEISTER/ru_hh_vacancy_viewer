import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Qt5Compat.GraphicalEffects

Page {
    id: mainPage
    property string paramsToContinue : ""
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
            id: historyRect
            anchors.centerIn: parent
            width: parent.width * 0.85
            height: parent.height * 0.85
            border.width: 1
            border.color: "#e1011e"
            radius: 10
            ListModel {
                id: historyModel
                Component.onCompleted: {
                    historyModel.clear();
                    historyModel.append(backend.getSearchHistories());
                }
            }
            ListView {
                id: historyView
                anchors.fill: parent
                anchors.margins: 50
                spacing: 30
                clip: true
                currentIndex: -1
                model: historyModel
                delegate: ItemDelegate {
                    id: historyBlock
                    width: historyRect.width - 100
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
                            anchors.rightMargin: 20
                            BusyIndicator {
                                Layout.alignment: Qt.AlignVCenter
                                id: updateStatus
                                running: false
                            }
                            Text {
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignVCenter
                                text: model.name
                                font.pointSize: 14
                                height: 50
                                wrapMode: Text.Wrap
                            }
                            Text {
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignVCenter
                                text: (model.found !== 0) ? (model.watched + " из " + model.found) : "Обновите"
                                font.pointSize: 14
                                wrapMode: Text.Wrap
                            }
                            Button {
                                Layout.alignment: Qt.AlignVCenter
                                text: qsTr("Продолжить")
                                highlighted: true
                                onClicked: {
                                    mainPage.paramsToContinue = model.params
                                    stackView.push("qrc:/pages/SearchPage.qml")
                                    headerPane.pageName = qsTr("Поиск")
                                }
                            }
                            Button {
                                Layout.alignment: Qt.AlignVCenter
                                text: qsTr("Обновить")
                                highlighted: true
                                onClicked: {
                                    updateStatus.running = true;
                                    backend.updateHistoryViewedCount(model.histId);
                                    historyModel.clear();
                                    historyModel.append(backend.getSearchHistories());
                                    updateStatus.running = false;
                                }
                            }
                            Button {
                                Layout.alignment: Qt.AlignVCenter
                                text: qsTr("Подробнее")
                                highlighted: true
                                onClicked: {
                                    historyView.currentIndex = index
                                    showHistorySearchParams.paramsList = backend.jsonParamsToVarList(model.params);
                                    showHistorySearchParams.open();
                                }
                            }
                            Button {
                                Layout.alignment: Qt.AlignVCenter
                                text: qsTr("Переименовать")
                                highlighted: true
                                onClicked: {
                                    historyView.currentIndex = index;
                                    showChangeHistoryName.open();
                                }
                            }
                            Button {
                                Layout.alignment: Qt.AlignVCenter
                                text: qsTr("Удалить")
                                highlighted: true
                                onClicked: {
                                    historyView.currentIndex = index
                                    showDeleteConfirm.open();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Dialog {
        property var paramsList : ListElement{}
        id: showHistorySearchParams
        width: 300
        height: 300
        x: (parent.width - width)/2
        y: (parent.height - height)/2
        Pane {
            anchors.fill: parent
            ColumnLayout {
                anchors.fill: parent
                Text {
                    text: qsTr("Параметры поиска")
                    font.bold: Font.Bold
                    font.pointSize: 12
                    Layout.alignment: Qt.AlignHCenter
                }
                Item {
                    id: paramsItem
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Flickable {
                        anchors.fill: parent
                        contentHeight: paramsFlow.height
                        contentWidth: paramsFlow.width
                        clip: true
                        ScrollIndicator.vertical: ScrollIndicator {
                            active: true
                        }
                        Flow {
                            id: paramsFlow
                            width: paramsItem.width
                            spacing: 5
                            ParamElement {
                                text: qsTr("Полный день")
                                visible: showHistorySearchParams.paramsList.fullday ? 1 : 0
                            }
                            ParamElement {
                                text: qsTr("Сменный график")
                                visible: showHistorySearchParams.paramsList.shift ? 1 : 0
                            }
                            ParamElement {
                                text: qsTr("Удаленная работа")
                                visible: showHistorySearchParams.paramsList.remote ? 1 : 0
                            }
                            ParamElement {
                                text: qsTr("Гибкий график")
                                visible: showHistorySearchParams.paramsList.agile ? 1 : 0
                            }
                            ParamElement {
                                text: qsTr("От 6 лет")
                                visible: showHistorySearchParams.paramsList.from6 ? 1 : 0
                            }
                            ParamElement {
                                text: qsTr("От 3 до 6 лет")
                                visible: showHistorySearchParams.paramsList.from3to6 ? 1 : 0
                            }
                            ParamElement {
                                text: qsTr("От 1 до 3 лет")
                                visible: showHistorySearchParams.paramsList.from1to3 ? 1 : 0
                            }
                            ParamElement {
                                text: qsTr("Без опыта")
                                visible: showHistorySearchParams.paramsList.unexp ? 1 : 0
                            }
                            ParamElement {
                                text: qsTr("Стажировка")
                                visible: showHistorySearchParams.paramsList.intern ? 1 : 0
                            }
                            ParamElement {
                                text: qsTr("Полная занятость")
                                visible: showHistorySearchParams.paramsList.fulltime ? 1 : 0
                            }
                            ParamElement {
                                text: qsTr("Частичная занятость")
                                visible: showHistorySearchParams.paramsList.fulltime ? 1 : 0
                            }
                            Repeater{
                                model: showHistorySearchParams.paramsList.cities
                                ParamElement {
                                    text: modelData.name
                                }
                            }
                            Repeater{
                                model: showHistorySearchParams.paramsList.tags
                                ParamElement {
                                    text: modelData.name
                                    color: modelData.type ? "#badbad" : "#e1011c"
                                    textColor: modelData.type ? "#000000" : "#ffffff"
                                }
                            }
                            Repeater{
                                model: showHistorySearchParams.paramsList.roles
                                ParamElement {
                                    text: modelData.name
                                }
                            }
                        }
                    }
                }
                Button{
                    text: qsTr("Закрыть")
                    highlighted: true
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: {
                        showHistorySearchParams.close();
                    }
                }
            }
        }
    }
    Dialog {
        id: showDeleteConfirm
        width: 300
        height: 200
        x: (parent.width - width)/2
        y: (parent.height - height)/2
        Pane {
            anchors.fill: parent
            ColumnLayout {
                anchors.fill: parent
                Text {
                    text: qsTr("Удаление")
                    font.bold: Font.Bold
                    font.pointSize: 12
                    Layout.alignment: Qt.AlignHCenter
                }
                Text{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    text: qsTr("Удалить ") + (historyView.currentIndex !== -1 ? historyModel.get(historyView.currentIndex).name : "") + qsTr(" ?")
                    font.pointSize: 14
                    font.bold: Font.Bold
                    wrapMode: Text.WordWrap
                }
                RowLayout {
                    Layout.fillWidth: true
                    Button {
                        Layout.fillWidth: true
                        text: qsTr("Нет")
                        highlighted: true
                        onClicked: {
                            showDeleteConfirm.close();
                        }
                    }
                    Button {
                        Layout.fillWidth: true
                        text: qsTr("Да")
                        highlighted: true
                        onClicked: {
                            backend.deleteHistory(historyModel.get(historyView.currentIndex).histId);
                            historyModel.remove(historyView.currentIndex);
                            historyView.currentIndex = -1;
                            showDeleteConfirm.close();
                        }
                    }
                }
            }
        }
    }
    Dialog {
        id: showChangeHistoryName
        width: 300
        height: 200
        x: (parent.width - width)/2
        y: (parent.height - height)/2
        Pane {
            anchors.fill: parent
            ColumnLayout {
                anchors.fill: parent
                Text {
                    text: qsTr("Переименование")
                    font.bold: Font.Bold
                    font.pointSize: 12
                    Layout.alignment: Qt.AlignHCenter
                }
                TextField {
                    id: currentHistName
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    text: (historyView.currentIndex !== -1 ? historyModel.get(historyView.currentIndex).name : "")
                    font.pointSize: 14
                }
                RowLayout {
                    Layout.fillWidth: true
                    Button {
                        Layout.fillWidth: true
                        highlighted: true
                        text: qsTr("Закрыть");
                        onClicked: {
                            showChangeHistoryName.close();
                        }
                    }
                    Button {
                        Layout.fillWidth: true
                        highlighted: true
                        text: qsTr("Ок")
                        onClicked: {
                            backend.renameHistory(currentHistName.text, historyModel.get(historyView.currentIndex).histId);
                            historyModel.clear();
                            historyModel.append(backend.getSearchHistories());
                            showChangeHistoryName.close();
                        }
                    }
                }
            }
        }
    }
}
