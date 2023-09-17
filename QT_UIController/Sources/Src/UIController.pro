TEMPLATE = app

QT += charts qml quick

CONFIG += c++11 link_pkgconfig
CONFIG +=lang-all

static {
    QT += svg
    QTPLUGIN += qtvirtualkeyboardplugin
}
SOURCES += Src/*.cpp

LIBS += C:\Qt\Qt5.9.1\Tools\mingw530_32\i686-w64-mingw32\lib\libws2_32.a

RESOURCES += qml.qrc \
    img.qrc

TRANSLATIONS = Languages/UIController_de_DE.ts Languages/UIController_en_US.ts Languages/UIController_es_ES.ts Languages/UIController_fr_FR.ts Languages/UIController_it_IT.ts Languages/UIController_ja_JP.ts Languages/UIController_ko_KR.ts Languages/UIController_zh_CN.ts Languages/UIController_sk_SK.ts Languages/UIController_da_DK.ts Languages/UIController_Traditional_zh_CN.ts

TESTDATA = $$PWD/data/*
DEFINES += TESTDATA_DIR=\\\"$$_PRO_FILE_PWD_/data\\\"
DEFINES += SRC_DIR=\\\"$$PWD\\\"
DEFINES += QT_MESSAGELOGCONTEXT

CONFIG+=disable-layouts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH +=C:\Qt\Qt5.9.1\Tools\QtCreator\bin\qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += Header/*.h

disable-xcb {
    message("The disable-xcb option has been deprecated. Please use disable-desktop instead.")
    CONFIG += disable-desktop
}
