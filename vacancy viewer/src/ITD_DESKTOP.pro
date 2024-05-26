QT += quick quickcontrols2 sql network widgets

CONFIG += c++17

INCLUDEPATH += "C:/Program Files/Python310/include"
INCLUDEPATH += "C:\Program Files\Python310"
INCLUDEPATH += C:\C++\libs\boost_1_82_0

LIBS += -L"C:\Program Files\Python310\libs" -lpython310 -lpython3
LIBS += -LC:\C++\libs\boost_1_82_0\stage142\lib -lboost_python310-vc142-mt-gd-x64-1_82

SOURCES += \
        backend.cpp \
        backendworker.cpp \
        classificator.cpp \
        database.cpp \
        main.cpp \
        programm.cpp \
        remotedatabase.cpp

resources.files = main.qml
resources.prefix = /$${TARGET}
RESOURCES += resources
RESOURCES += \
        classify \
        pages/ \
        icons/

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
        classify \
        pages/AboutPage.qml \
        pages/HistoryPage.qml \
        pages/ParamElement.qml \
        pages/PreferencesPage.qml \
        pages/ProfilePage.qml \
        pages/SearchPage.qml \
        qtquickcontrols2.conf \
        pages/shader.frag.qsb

HEADERS += \
        backend.h \
        backendworker.h \
        classificator.h \
        database.h \
        main.h \
        programm.h \
        remotedatabase.h

RC_ICONS = icons/itd-logo.ico
