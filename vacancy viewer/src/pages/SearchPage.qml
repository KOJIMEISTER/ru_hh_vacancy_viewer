import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Qt5Compat.GraphicalEffects

Page {
    id: searchPage
    Drawer {
        property int vacId: 0;
        property alias textVacName: textVacName.text
        property alias infoTags : infoTags
        property alias description : description.text
        id: infoPanel
        width: parent.width * 0.5
        height: parent.height
        edge: Qt.RightEdge
        dim: false
        onClosed: {
            vacancyList.currentIndex = -1;
        }
        Pane {
            anchors.fill: parent
            Item {
                anchors.fill: parent
                anchors.margins: 10
                ColumnLayout {
                    id: vacContent
                    anchors.fill: parent
                    spacing: 10
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10
                        Image {
                            source: "qrc:/icons/external-link.svg"
                            sourceSize.height: 25
                            sourceSize.width: 25
                        }
                        Text {
                            id: textVacName
                            font.pointSize: 24
                            font.bold: Font.Bold
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            onTextChanged: {
                                descriptionScroll.contentY = 0;
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    Qt.openUrlExternally("https://hh.ru/vacancy/" + infoPanel.vacId)
                                }
                            }
                        }
                    }
                    Flow {
                        Layout.fillWidth: true
                        spacing: 5
                        Repeater {
                            model: ListModel {
                                id: infoTags
                            }
                            Rectangle {
                                width: tagTextMetrics.tightBoundingRect.width + 20
                                height: 25
                                radius: 10
                                color: "#DCDCDC"
                                clip: true
                                Text {
                                    anchors.centerIn: parent
                                    id: tagNameInfo
                                    text: model.name
                                    font.pointSize: 12
                                }
                                TextMetrics {
                                    id: tagTextMetrics
                                    font: tagNameInfo.font
                                    text: tagNameInfo.text
                                }
                            }

                        }
                    }
                    Flickable {
                        id: descriptionScroll
                        clip: true
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        contentHeight: description.height
                        contentWidth: description.width
                        boundsBehavior: Flickable.StopAtBounds
                        Text {
                            id: description
                            width: vacContent.width
                            font.pointSize: 14
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            }
        }
    }
    Row {
        anchors.fill: parent
        Pane {
            id: searchCont
            width: 320
            height: parent.height
            NumberAnimation {
                id: closeAnim
                target: searchCont
                property: "width"
                duration: 200
                from: 300
                to: 35
                easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                id: openAnim
                target: searchCont
                property: "width"
                duration: 200
                from: 35
                to: 300
                easing.type: Easing.InOutQuad
            }
            Button {
                property bool isClosed: false
                anchors.left: rectSearchCont.right
                width: 20
                height: 50
                highlighted: true
                Image {
                    anchors.centerIn: parent
                    source: (parent.isClosed) ? "qrc:/icons/chevron-right.svg" : "qrc:/icons/chevron-left.svg"
                }
                onClicked: {
                    if(isClosed)
                    {
                        openAnim.start();
                        isClosed = false;
                    }
                    else
                    {
                        closeAnim.start();
                        isClosed = true;
                    }
                }
            }
            Rectangle {
                id: rectSearchCont
                width: parent.width - 20
                height: parent.height
                border.color: "#e1011e"
                border.width: 1
                radius: 10
                clip: true
                ColumnLayout {
                    id: columnSearch
                    anchors.fill: parent
                    anchors.margins: 10
                    Flickable {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        contentWidth: searchColumn.width
                        contentHeight: searchColumn.height
                        boundsBehavior: Flickable.StopAtBounds
                        clip: true
                        ColumnLayout {
                            id: searchColumn
                            spacing: 10
                            width: columnSearch.width
                            Component.onCompleted: {
                                if(stackView.depth === 3)
                                {
                                    var Item = stackView.get(1, StackView.DontLoad);
                                    if(Item !== null && Item.paramsToContinue !== "")
                                    {
                                        var paramsMap = backend.jsonParamsToVarList(Item.paramsToContinue);
                                        workScheduleShift.checked = paramsMap.shift;
                                        workScheduleRemote.checked = paramsMap.remote;
                                        workScheduleAgile.checked = paramsMap.agile;
                                        workScheduleFullDay.checked = paramsMap.fullday;
                                        workExpFrom6.checked = paramsMap.from6;
                                        workExpFrom3to6.checked = paramsMap.from3to6;
                                        workExp1to3.checked = paramsMap.from1to3;
                                        workExpNone.checked = paramsMap.unexp;
                                        workTypeIntern.checked = paramsMap.intern;
                                        workTypePartTime.checked = paramsMap.parttime;
                                        workTypeFullTime.checked = paramsMap.fulltime;
                                        tagsRepeater.model.clear();
                                        tagsRepeater.model.append(paramsMap.tags);
                                        citiesRepeater.model.clear();
                                        citiesRepeater.model.append(paramsMap.cities);
                                        roleRepeater.model.clear();
                                        roleRepeater.model.append(paramsMap.roles);
                                    }
                                }
                            }
                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: qsTr("Специализации")
                                font.bold: Font.Bold
                                font.pointSize: 12
                            }
                            Flow {
                                Layout.fillWidth: true
                                spacing: 5
                                Repeater {
                                    id: roleRepeater
                                    onItemAdded: {
                                        vacancyPreviewModel.clear();
                                    }
                                    onItemRemoved: {
                                        vacancyPreviewModel.clear();
                                    }
                                    model: ListModel {
                                        id: roleModel
                                    }
                                    Rectangle {
                                        property string name: model.name
                                        width: parent.width
                                        height: 50
                                        radius: 10
                                        border.color: "#e1011e"
                                        border.width: 1
                                        Image {
                                            source: "qrc:/icons/x-circle.svg"
                                            anchors.left: parent.left
                                            anchors.leftMargin: 5
                                            anchors.top: parent.top
                                            anchors.topMargin: 5
                                            MouseArea {
                                                anchors.fill: parent
                                                onClicked: {
                                                    for(var i = 0; i < roleModel.count; ++i)
                                                    {
                                                        if(roleModel.get(i).name === model.name)
                                                        {
                                                            roleModel.remove(i)
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        ColumnLayout {
                                            spacing: 5
                                            anchors.fill: parent
                                            anchors.margins: 25
                                            Text {
                                                id: dirName
                                                Layout.alignment: Qt.AlignHCenter
                                                text: model.name
                                                font.pointSize: 12
                                            }
                                        }
                                    }
                                }
                            }
                            Button {
                                id: addDirButton
                                Layout.fillWidth: true
                                highlighted: true
                                Image {
                                    id: addDirImage
                                    anchors.centerIn: parent
                                    anchors.margins: 5
                                    source: "qrc:/icons/plus.svg"
                                }
                                onClicked: {
                                    dialogAddRole.open();
                                }
                            }
                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: qsTr("Города")
                                font.bold: Font.Bold
                                font.pointSize: 12
                            }
                            Flow {
                                Layout.fillWidth: true
                                spacing: 5
                                Repeater {
                                    id: citiesRepeater
                                    onItemAdded: {
                                        vacancyPreviewModel.clear();
                                    }
                                    onItemRemoved: {
                                        vacancyPreviewModel.clear();
                                    }
                                    model: ListModel {
                                        id: citiesModel
                                    }
                                    Rectangle {
                                        property string name: model.name
                                        width: textCityMetrica.tightBoundingRect.width + 20
                                        height: 25
                                        color: "#DCDCDC"
                                        radius: 5
                                        Text {
                                            id: cityName
                                            text: model.name
                                            font.pointSize: 12
                                            anchors.centerIn: parent
                                            wrapMode: Text.WrapAnywhere
                                        }
                                        TextMetrics {
                                            id: textCityMetrica
                                            font: cityName.font
                                            text: cityName.text
                                        }
                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: {
                                                for(var i = 0; i < citiesModel.count; ++i)
                                                {
                                                    if(citiesModel.get(i).name === cityName.text)
                                                    {
                                                        citiesModel.remove(i)
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            Button {
                                id: addCityButton
                                Layout.fillWidth: true
                                highlighted: true
                                Image {
                                    id: addCityImage
                                    anchors.centerIn: parent
                                    anchors.margins: 5
                                    source: "qrc:/icons/plus.svg"
                                }
                                onClicked: {
                                    dialogAddCity.open();
                                }
                            }
                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: qsTr("Ключевые навыки")
                                font.pointSize: 12
                                font.bold: Font.Bold
                            }
                            Flow {
                                Layout.fillWidth: true
                                spacing: 5
                                Repeater {
                                    id: tagsRepeater
                                    onItemAdded: {
                                        vacancyPreviewModel.clear();
                                    }
                                    onItemRemoved: {
                                        vacancyPreviewModel.clear();
                                    }
                                    model: ListModel {
                                        id: tagsModel
                                    }
                                    Rectangle {
                                        property string name: model.name
                                        property bool type: model.type
                                        width: textMetrica.tightBoundingRect.width + 20
                                        height: 25
                                        color: model.type ? "#badbad" : "#e1011c"
                                        radius: 5
                                        Text {
                                            id: tagName
                                            text: model.name
                                            font.pointSize: 12
                                            anchors.centerIn: parent
                                            wrapMode: Text.WrapAnywhere
                                            color: model.type ? "#000000" : "#ffffff"
                                        }
                                        TextMetrics {
                                            id: textMetrica
                                            font: tagName.font
                                            text: tagName.text
                                        }
                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: {
                                                for(var i = 0; i < tagsModel.count; ++i)
                                                {
                                                    if(tagsModel.get(i).name === tagName.text)
                                                    {
                                                        tagsModel.remove(i)
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            Button {
                                id: addTagButton
                                Layout.fillWidth: true
                                highlighted: true
                                Image {
                                    id: addImage
                                    anchors.centerIn: parent
                                    anchors.margins: 5
                                    source: "qrc:/icons/plus.svg"
                                }
                                onClicked: {
                                    dialogAddTags.open();
                                }
                            }
                            CheckBox {
                                id: showSeen
                                onCheckedChanged: {
                                    vacancyPreviewModel.clear();
                                }
                                Layout.fillWidth: true
                                text: qsTr("Показывать просмотренные")
                                font.pointSize: 12
                            }
                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: qsTr("Тип занятости")
                                font.bold: Font.Bold
                                font.pointSize: 12
                            }
                            CheckBox {
                                id: workTypeFullTime
                                onCheckedChanged: {
                                    vacancyPreviewModel.clear();
                                }
                                Layout.fillWidth: true
                                text: qsTr("Полная занятость")
                                font.pointSize: 12
                            }
                            CheckBox {
                                id: workTypePartTime
                                onCheckedChanged: {
                                    vacancyPreviewModel.clear();
                                }
                                Layout.fillWidth: true
                                text: qsTr("Частичная занятость")
                                font.pointSize: 12
                            }
                            CheckBox {
                                id: workTypeIntern
                                onCheckedChanged: {
                                    vacancyPreviewModel.clear();
                                }
                                Layout.fillWidth: true
                                text: qsTr("Стажировка")
                                font.pointSize: 12
                            }
                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: qsTr("Опыт работы")
                                font.bold: Font.Bold
                                font.pointSize: 12
                            }
                            CheckBox {
                                id: workExpNone
                                onCheckedChanged: {
                                    vacancyPreviewModel.clear();
                                }
                                Layout.fillWidth: true
                                text: qsTr("Без опыта")
                                font.pointSize: 12
                            }
                            CheckBox {
                                id: workExp1to3
                                onCheckedChanged: {
                                    vacancyPreviewModel.clear();
                                }
                                Layout.fillWidth: true
                                text: qsTr("От 1 года до 3")
                                font.pointSize: 12
                            }
                            CheckBox {
                                id: workExpFrom3to6
                                onCheckedChanged: {
                                    vacancyPreviewModel.clear();
                                }
                                Layout.fillWidth: true
                                text: qsTr("От 3 лет до 6")
                                font.pointSize: 12
                            }
                            CheckBox {
                                id: workExpFrom6
                                onCheckedChanged: {
                                    vacancyPreviewModel.clear();
                                }
                                Layout.fillWidth: true
                                text: qsTr("От 6 лет")
                                font.pointSize: 12
                            }
                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: qsTr("График работы")
                                font.bold: Font.Bold
                                font.pointSize: 12
                            }
                            CheckBox {
                                id: workScheduleFullDay
                                onCheckedChanged: {
                                    vacancyPreviewModel.clear();
                                }
                                Layout.fillWidth: true
                                text: qsTr("Полный день")
                                font.pointSize: 12
                            }
                            CheckBox {
                                id: workScheduleAgile
                                onCheckedChanged: {
                                    vacancyPreviewModel.clear();
                                }
                                Layout.fillWidth: true
                                text: qsTr("Гибкий график")
                                font.pointSize: 12
                            }
                            CheckBox {
                                id: workScheduleRemote
                                onCheckedChanged: {
                                    vacancyPreviewModel.clear();
                                }
                                Layout.fillWidth: true
                                text: qsTr("Удаленная работа")
                                font.pointSize: 12
                            }
                            CheckBox {
                                id: workScheduleShift
                                onCheckedChanged: {
                                    vacancyPreviewModel.clear();
                                }
                                Layout.fillWidth: true
                                text: qsTr("Сменный график")
                                font.pointSize: 12
                            }
                        }
                    }
                    Button {
                        id: buttonSearch
                        Layout.fillWidth: true
                        text: qsTr("Поиск")
                        highlighted: true
                        onClicked: {
                            var params = "{";
                            params += '"shift":' + workScheduleShift.checked.toString() + ",";
                            params += '"remote":' + workScheduleRemote.checked.toString() + ",";
                            params += '"agile":' + workScheduleAgile.checked.toString() + ",";
                            params += '"fullday":' + workScheduleFullDay.checked.toString() + ",";
                            params += '"from6":' + workExpFrom6.checked.toString() + ",";
                            params += '"from3to6":' + workExpFrom3to6.checked.toString() + ",";
                            params += '"from1to3":' + workExp1to3.checked.toString() + ",";
                            params += '"unexp":' + workExpNone.checked.toString() + ",";
                            params += '"intern":' + workTypeIntern.checked.toString() + ",";
                            params += '"parttime":' + workTypePartTime.checked.toString() + ",";
                            params += '"fulltime":' + workTypeFullTime.checked.toString() + ",";
                            params += '"tags":[';
                            for(var i = 0; i < tagsRepeater.count; ++i)
                            {
                                params += '{';
                                params += '"name":"' + tagsRepeater.itemAt(i).name;
                                params += '","type":' + tagsRepeater.itemAt(i).type.toString();
                                params += '}';
                                if(i == tagsRepeater.count-1)
                                {
                                    break;
                                }
                                else
                                {
                                    params += ',';
                                }
                            }
                            params += '],"cities":[';
                            for(i = 0; i < citiesRepeater.count; ++i)
                            {
                                params += '{';
                                params += '"name":"' + citiesRepeater.itemAt(i).name;
                                params += '"}';
                                if(i == citiesRepeater.count-1)
                                {
                                    break;
                                }
                                else
                                {
                                    params += ',';
                                }
                            }
                            params += '],"roles":[';
                            for(i = 0; i < roleRepeater.count; ++i)
                            {
                                params += '{';
                                params += '"name":"' + roleRepeater.itemAt(i).name;
                                params += '"}';
                                if(i == roleRepeater.count-1)
                                {
                                    break;
                                }
                                else
                                {
                                    params += ',';
                                }
                            }
                            params += ']}';
                            vacancyPreviewModel.clear();
                            vacancyPreviewModel.append(backend.getVacToPrepareBySearch(params, showSeen.checked, orderType.cending));
                        }
                    }
                }
            }

        }
        ColumnLayout {
            width: parent.width - 300
            height: parent.height
            Pane {
                id: searchPanel
                Layout.fillWidth: true
                height: 40
                RowLayout{
                    anchors.leftMargin: 30
                    anchors.rightMargin: 30
                    anchors.fill: parent
                    Text {
                        text: "Сортировать по датам"
                        font.pointSize: 16
                        Layout.alignment: Qt.AlignVCenter
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                orderType.cending = !orderType.cending;
                                if(vacancyPreviewModel.count !== 0)
                                {
                                    buttonSearch.clicked();
                                }
                            }
                        }
                    }
                    Image {
                        id: orderType
                        property bool cending: true
                        source: cending ? "qrc:/icons/arrow-down.svg" : "qrc:/icons/arrow-up.svg"
                        sourceSize.height: 22
                        sourceSize.width: 22
                        Layout.alignment: Qt.AlignVCenter
                    }
                    Text {
                        text: "Найдено " + vacancyPreviewModel.count
                        opacity: vacancyPreviewModel.count === 0 ? 0.0 : 1.0
                        font.pointSize: 16
                        font.bold: Font.Bold
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter
                    }
                    RoundButton {
                        id: atListBegining;
                        Layout.alignment: Qt.AlignVCenter
                        onClicked: {
                            vacancyList.positionViewAtBeginning();
                        }
                        opacity: vacancyList.contentY === 0 ? 0.0 : 1.0
                        highlighted: true
                        Image {
                            anchors.centerIn: parent
                            source: "qrc:/icons/chevron-up.svg"
                            sourceSize.width: parent.height - 10
                            sourceSize.height: parent.height - 10
                        }
                    }
                }
            }
            Pane {
                Layout.fillWidth: true
                Layout.fillHeight: true
                ListModel {
                    id: vacancyPreviewModel
                }
                ListView {
                    id: vacancyList
                    anchors.fill: parent
                    anchors.margins: 30
                    spacing: 20
                    currentIndex: -1
                    clip: true
                    boundsBehavior: ListView.StopAtBounds
                    cacheBuffer: 0
                    Connections {
                        target: backend
                        function onSendVacPreview(VacPreview, Index)
                        {
                            vacancyPreviewModel.set(Index, {"content":VacPreview, "loaded":true, "viewed":vacancyPreviewModel.get(Index).viewed, "predict":""});
                            backend.makeVacToPredictRequest(Index, vacancyPreviewModel.get(Index).content.vacId, vacancyPreviewModel.get(Index).content.roleId);
                        }
                    }
                    Connections {
                        target: backend
                        function onSendVacPredict(Index, Predict)
                        {
                            vacancyPreviewModel.get(Index).predict = Predict;
                        }
                    }
                    model: vacancyPreviewModel
                    delegate: ItemDelegate {
                        width: vacancyList.width
                        height: 270
                        background: Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                        }
                        Component.onCompleted: {
                            if(!vacancyPreviewModel.get(index).loaded)
                            {
                                backend.makeVacPreviewInfoRequest(model.content.vacId, index);
                            }
                        }
                        BusyIndicator {
                            id: loadingStatusIndicator
                            anchors.fill: parent
                            anchors.margins: 10
                            running: true
                        }
                        Loader {
                            anchors.fill: parent
                            sourceComponent: vacPreview
                            active: model.loaded
                            onLoaded: {
                                loadingStatusIndicator.running = false;
                            }
                        }
                        Component {
                            id: vacPreview
                            Rectangle {
                                anchors.fill: parent
                                anchors.margins: 15
                                border.width: 3
                                border.color: vacancyList.currentIndex == index ? "#e1011c" : model.viewed ? "#8b00ff" : "#DCDCDC"
                                radius: 10
                                Item {
                                    id: vacancyBlock
                                    property var predicts: model.content.roles
                                    anchors.fill: parent
                                    anchors.margins: 15
                                    Column {
                                        anchors.fill: parent
                                        spacing: 5
                                        Text {
                                            width: parent.width
                                            text: model.content.vacName
                                            font.pointSize: 22
                                            font.bold: Font.Bold
                                            clip: true
                                        }
                                        Row {
                                            width: parent.width
                                            height: 30
                                            spacing: 10
                                            Text {
                                                anchors.verticalCenter: parent.verticalCenter
                                                text: "от " + model.content.paymentFrom
                                                font.pointSize: 22
                                                visible: model.content.paymentFrom !== "0" ? 1.0 : 0.0
                                            }
                                            Text {
                                                anchors.verticalCenter: parent.verticalCenter
                                                text: "до " + model.content.paymentTo
                                                visible: model.content.paymentTo !== "0" ? 1.0 : 0.0
                                                font.pointSize: 22
                                            }
                                            Text {
                                                anchors.verticalCenter: parent.verticalCenter
                                                text: model.content.paymentCur + (model.content.paymentCur === "RUR") ? "РУБ" : ""
                                                visible: model.content.paymentCur !== "" ? 1.0 : 0.0
                                                font.pointSize: 22
                                            }
                                            Text {
                                                anchors.verticalCenter: parent.verticalCenter
                                                text: model.content.paymentGross ? "после вычета" : " до вычета"
                                                visible: model.content.paymentCur !== "" ? 1.0 : 0.0
                                                font.pointSize: 22
                                            }
                                        }
                                        Text {
                                            text: model.content.experience === "noExperience" ? "Без опыта" :
                                                                                                model.content.experience === "between1And3" ? "От 1 до 3 лет" :
                                                                                                                                              model.content.experience === "between3And6" ? "От 3 до 6 лет" :  "От 6 лет"
                                            font.pointSize: 12
                                        }
                                        Text {
                                            text: model.content.schedule === "flexible" ? "Гибкий график" :
                                                                                          model.content.schedule === "fullDay" ? "Полный день" : "Удаленная работа"
                                            font.pointSize: 12
                                        }
                                        Text {
                                            text: model.content.employment === "full" ? "Полная занятость" :
                                                                                        model.content.employment === "part" ? "Частичная занятость" :
                                                                                                                              model.content.employment === "probation" ? "Стажировка" : "Проектная работа"
                                            font.pointSize: 12
                                        }
                                        Text {
                                            text: model.content.area
                                            font.pointSize: 12
                                        }
                                        Rectangle {
                                            visible: model.predict === "" ? 0.0 : 1.0
                                            width: textMetricaPredict.tightBoundingRect.width + 20
                                            height: 20
                                            border.width: 1
                                            border.color: "#DCDCDC"
                                            radius: 10
                                            Text {
                                                id: dirName
                                                text: model.predict
                                                font.pointSize: 12
                                                anchors.centerIn: parent
                                            }
                                            TextMetrics {
                                                id: textMetricaPredict
                                                font: dirName.font
                                                text: dirName.text
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        onClicked: {
                            vacancyList.currentIndex = index
                            infoPanel.description = backend.getVacancyDescription(model.content.vacId);
                            infoPanel.vacId = model.content.vacId;
                            infoPanel.textVacName = model.content.vacName;
                            infoPanel.infoTags.clear();
                            infoPanel.infoTags.append(backend.getVacancyTags(model.content.vacId));
                            infoPanel.open();
                        }
                    }
                }
            }
        }
    }
    Dialog {
        id: dialogAddTags
        width: 300
        height: 350
        x: 300
        y: 70
        Pane {
            anchors.fill: parent
            ColumnLayout {
                anchors.fill: parent
                spacing: 5
                Item {
                    Layout.fillWidth: true
                    height: 30
                    Rectangle {
                        width: 50
                        height: 30
                        color: "#badbad"
                        radius: 10
                        anchors.left: parent.left
                        anchors.top: parent.top
                        Text {
                            anchors.centerIn: parent
                            text: qsTr("ЛКМ")
                        }
                    }
                    Rectangle {
                        width: 50
                        height: 30
                        color: "#e1011e"
                        radius: 10
                        anchors.right: parent.right
                        anchors.top: parent.top
                        Text {
                            anchors.centerIn: parent
                            text: qsTr("ПКМ")
                            color: "#ffffff"
                        }
                    }
                }
                TextField {
                    id: textFieldTagsName
                    property bool isSearching: false
                    placeholderText: qsTr("Название тега")
                    height: 30
                    Layout.fillWidth: true
                    onDisplayTextChanged: {
                        if(!isSearching)
                        {
                            isSearching = true;
                            if(foundTagsModel.count !== 0)
                            {
                                foundTagsModel.clear();
                            }
                            foundTagsModel.append(backend.getSimilarTags(textFieldTagsName.text));
                            isSearching = false;
                        }
                    }
                    onPressed: {
                        if(!isSearching)
                        {
                            isSearching = true;
                            if(foundTagsModel.count !== 0)
                            {
                                foundTagsModel.clear();
                            }
                            foundTagsModel.append(backend.getSimilarTags(""));
                            isSearching = false;
                        }
                    }
                }
                ListView {
                    id: viewTags
                    clip: true
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    model: ListModel {
                        id: foundTagsModel
                    }
                    delegate: ItemDelegate {
                        width: viewTags.width
                        clip: true
                        height: 50
                        RowLayout {
                            anchors.fill: parent
                            Text{
                                text: model.name
                                Layout.fillWidth: true
                                wrapMode: Text.WordWrap
                                font.pointSize: 12
                            }
                            Text{
                                text: model.count
                                font.pointSize: 12
                            }
                        }
                        MouseArea{
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            onClicked: mouse => {
                                           var founded = false
                                           for(var i = 0; i < tagsModel.count; ++i)
                                           {
                                               if(tagsModel.get(i).name === model.name)
                                               {
                                                   founded = true;
                                                   break;
                                               }
                                           }
                                           if(mouse.button === Qt.RightButton)
                                           {
                                               if(!founded)
                                               {
                                                   tagsModel.append({"name":model.name, "type":false})
                                               }
                                           }
                                           else if (mouse.button === Qt.LeftButton)
                                           {
                                               if(!founded)
                                               {
                                                   tagsModel.append({"name":model.name, "type":true})
                                               }
                                           }
                                       }
                        }
                    }
                }
                RoundButton {
                    Layout.alignment: Qt.AlignHCenter
                    highlighted: true
                    onClicked: {
                        dialogAddTags.close();
                    }
                    Image {
                        anchors.fill: parent
                        anchors.margins: 10
                        source: "qrc:/icons/x.svg"
                    }
                }
            }
        }

    }
    Dialog {
        id: dialogAddCity
        width: 300
        height: 250
        x: 300
        y: 70
        ColumnLayout {
            anchors.fill: parent
            spacing: 15
            TextField {
                id: textFieldCity
                property bool isSearching: false
                placeholderText: qsTr("Название города")
                height: 30
                Layout.fillWidth: true
                onDisplayTextChanged: {
                    if(!isSearching)
                    {
                        isSearching = true;
                        if(foundCitiesModel.count !== 0)
                        {
                            foundCitiesModel.clear();
                        }
                        foundCitiesModel.append(backend.getSimilarCities(textFieldCity.text));
                        isSearching = false;
                    }
                }
                onPressed: {
                    if(!isSearching)
                    {
                        isSearching = true;
                        if(foundCitiesModel.count !== 0)
                        {
                            foundCitiesModel.clear();
                        }
                        foundCitiesModel.append(backend.getSimilarCities(""));
                        isSearching = false;
                    }
                }
            }
            ListView {
                id: viewCity
                clip: true
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: ListModel {
                    id: foundCitiesModel
                }
                delegate: ItemDelegate {
                    width: viewCity.width
                    height: 50
                    RowLayout {
                        anchors.fill: parent
                        Text{
                            text: model.name
                            font.pointSize: 12
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                        }
                        Text{
                            text: model.count
                            font.pointSize: 12
                        }
                    }
                    onClicked: {
                        var founded = false
                        for(var i = 0; i < citiesModel.count; ++i)
                        {
                            if(citiesModel.get(i).name === model.name)
                            {
                                founded = true;
                                break;
                            }
                        }
                        if(!founded)
                        {
                            citiesModel.append({"name":model.name})
                        }
                    }
                }
            }
            RoundButton {
                Layout.alignment: Qt.AlignHCenter
                highlighted: true
                height: contentHeight
                onClicked: {
                    dialogAddCity.close();
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
        id: dialogAddRole
        width: 300
        height: 250
        x: 300
        y: 70
        ColumnLayout {
            id: columnRoles
            anchors.fill: parent
            spacing: 10
            ListView {
                id: viewRoles
                clip: true
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: ListModel {
                    id: foundRolesModel
                    Component.onCompleted: {
                        foundRolesModel.clear();
                        foundRolesModel.append(backend.getAllRoles());
                    }
                }
                delegate: ItemDelegate {
                    width: viewRoles.width
                    height: 50
                    clip: true
                    RowLayout {
                        anchors.fill: parent
                        Text{
                            text: model.name
                            font.pointSize: 12
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                        }
                        Text{
                            text: model.count
                            font.pointSize: 12
                            wrapMode: Text.WordWrap
                        }
                    }
                    onClicked: {
                        var founded = false
                        for(var i = 0; i < roleModel.count; ++i)
                        {
                            if(roleModel.get(i).name === model.name)
                            {
                                founded = true;
                                break;
                            }
                        }
                        if(!founded)
                        {
                            roleModel.append({"name":model.name})
                        }
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
}
