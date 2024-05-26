QT += quick quickcontrols2 sql network widgets

CONFIG += c++20 c17

INCLUDEPATH += "C:\\Program Files\\Python310\\include"
INCLUDEPATH += "C:\\Program Files\\Python310"
INCLUDEPATH += "C:\\C++\\libs\\boost_1_82_0"
INCLUDEPATH += "C:\\C++\\libs\\x64-libcurl-static\\include"

LIBS += -L"C:\\Program Files\\Python310\\libs" -lpython310 -lpython3
LIBS += -L"C:\\C++\\libs\\boost_1_82_0\\stage142\\lib" -lboost_python310-vc142-mt-gd-x64-1_82
LIBS += -L"C:\\C++\\libs\\x64-libcurl-static\\lib" -llibcurl
LIBS += -L"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0\\um\\x64" -lWs2_32 -lCrypt32 -lWldap32 -lNormaliz -ladvapi32


SOURCES += \
        backend.cpp \
        interpreter/interpreter.cpp \
        loader/loaderprog.cpp \
        loader/network.cpp \
        loader/queryhandler.cpp \
        loader/urlparams.cpp \
        loader/vacloader.cpp \
        loader/vacverifier.cpp \
        main.cpp \
        remotedatabase.cpp

resources.files = main.qml
resources.prefix = /$${TARGET}
RESOURCES += resources \
        icons \
        pages

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
        pages/DatasetPage.qml \
        pages/MainPage.qml \
        pages/MarkupPage.qml \
        pages/RefreshPage.qml \
        pages/TrainPage.qml

HEADERS += \
        backend.h \
        interpreter/interpreter.h \
        loader/loaderprog.h \
        loader/network.h \
        loader/queryhandler.h \
        loader/urlparams.h \
        loader/vacloader.h \
        loader/vacverifier.h \
        loader/vacancy.hpp \
        remotedatabase.h

RC_ICONS = icons/itd-logo.ico
