import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

ApplicationWindow {
    id: window
    width: 1280
    height: 720
    visible: true
    title: qsTr("Быстрый поиск вакансий")
    property real globalTime : 0.0
    Timer {
        interval: 1
        running: true
        repeat: true
        onTriggered: {
            window.globalTime += 0.005;
        }
    }
    header: headerPane
    Pane {
        id: headerPane
        width: parent.width
        height: 50
        property alias pageName: pageName.text
        RoundButton {
            width: 40
            height: 40
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 40
            highlighted: true
            Image {
                id: menuImage
                anchors.fill: parent
                anchors.margins: 10
                source: "qrc:/icons/menu.svg"
            }
            onClicked: {
                sideMenu.open();
            }
        }
        Text {
            id: pageName
            text: qsTr("Главная")
            anchors.centerIn: parent
            font.bold: Font.Bold
            font.pointSize: 22
        }
    }

    Drawer {
        id: sideMenu
        width: 225
        height: window.height
        Column {
            anchors.fill: parent
            ListView {
                id: sideMenuList
                height: parent.height
                width: parent.width
                focus: true
                currentIndex: -1
                delegate: ItemDelegate {
                    height: 50
                    width: parent.width
                    Row {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 20
                        Image {
                            id: tabImage
                            source: model.image
                            sourceSize.height: parent.height
                            sourceSize.width: parent.height
                        }
                        Text {
                            text: model.title
                            font.pointSize: 16
                        }
                    }
                    onClicked: {
                        sideMenuList.currentIndex = index;
                        while(stackView.depth !== 1)
                        {
                            stackView.pop();
                        }
                        stackView.push(model.source);
                        sideMenu.close();
                        headerPane.pageName = model.title;
                    }
                }
                model: ListModel {
                    id: sideMenuListModel
                    ListElement { title: "Главная"; source: "qrc:/pages/MainPage.qml"; image: "qrc:/icons/home.svg";}
                    ListElement { title: "Профиль"; source: "qrc:/pages/ProfilePage.qml"; image: "qrc:/icons/user.svg";}
                    ListElement { title: "Поиск"; source: "qrc:/pages/SearchPage.qml"; image: "qrc:/icons/search.svg";}
                    ListElement { title: "История"; source: "qrc:/pages/HistoryPage.qml"; image: "qrc:/icons/book-open.svg";}
                    ListElement { title: "Настройки"; source: "qrc:/pages/PreferencesPage.qml"; image: "qrc:/icons/settings.svg";}
                    ListElement { title: "О программе"; source: "qrc:/pages/AboutPage.qml"; image: "qrc:/icons/info.svg";}
                }
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "qrc:/pages/MainPage.qml"
    }
}
