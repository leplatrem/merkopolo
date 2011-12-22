#-------------------------------------------------
#
# Merkaartor dependencies
#
#-------------------------------------------------
MERKAARTOR_DIR = $$PWD/../merkaartor
MERKAARTOR_SRC_DIR = $$MERKAARTOR_DIR/src

include($$MERKAARTOR_DIR/3rdparty/qtsingleapplication-2.6_1-opensource/src/qtsingleapplication.pri)
include($$MERKAARTOR_DIR/3rdparty/qttoolbardialog-2.2_1-opensource/src/qttoolbardialog.pri)

INCLUDEPATH += $$MERKAARTOR_SRC_DIR $$MERKAARTOR_DIR/include $$MERKAARTOR_DIR/include/builtin-ggl $$MERKAARTOR_DIR/interfaces
DEPENDPATH += $$MERKAARTOR_SRC_DIR $$MERKAARTOR_DIR/include $$MERKAARTOR_DIR/include/builtin-ggl $$MERKAARTOR_DIR/interfaces

include($$MERKAARTOR_DIR/interfaces/Interfaces.pri)
include($$MERKAARTOR_SRC_DIR/common/common.pri)
include($$MERKAARTOR_SRC_DIR/Utils/Utils.pri)
include($$MERKAARTOR_SRC_DIR/Backend/Backend.pri)
include($$MERKAARTOR_SRC_DIR/Commands/Commands.pri)
include($$MERKAARTOR_SRC_DIR/Render/Render.pri)
include($$MERKAARTOR_SRC_DIR/Layers/Layers.pri)
include($$MERKAARTOR_SRC_DIR/Features/Features.pri)
include($$MERKAARTOR_SRC_DIR/Interactions/Interactions.pri)
include($$MERKAARTOR_SRC_DIR/Preferences/Preferences.pri)
include($$MERKAARTOR_SRC_DIR/PaintStyle/PaintStyle.pri)
include($$MERKAARTOR_SRC_DIR/PaintStyle/PaintStyleEditor.pri)

# Useless but dependencies...
include($$MERKAARTOR_SRC_DIR/Docks/Docks.pri)
include($$MERKAARTOR_SRC_DIR/ImportExport/ImportExport.pri)  # from DownloadOSM
include($$MERKAARTOR_SRC_DIR/GPS/GPS.pri) # from MapView
include($$MERKAARTOR_SRC_DIR/TagTemplate/TagTemplate.pri)  # from ShortcutOverrideFilter
include($$MERKAARTOR_SRC_DIR/Sync/Sync.pri)  # from DocumentCommands
include($$MERKAARTOR_SRC_DIR/NameFinder/NameFinder.pri)  # from GotoDialog
include($$MERKAARTOR_SRC_DIR/QMapControl.pri)  # from ImageMapLayer
include($$MERKAARTOR_SRC_DIR/qextserialport/qextserialport.pri)  # from QGPSDevice
include($$MERKAARTOR_SRC_DIR/Tools/Tools.pri)  # from MainWindow


CONFIG += rtti stl

# External dependancies
win32 {
    LIBS += -L$$(QTDIR)bin
    win32-g++:LIBS += -lgdal
}
else {
    LIBS += $$system(gdal-config --libs)
    QMAKE_CXXFLAGS += $$system(gdal-config --cflags)
    QMAKE_CFLAGS += $$system(gdal-config --cflags)
}

LIBS += -lproj

QT  += core gui xml svg network webkit

SOURCES += MainWindow.cpp
HEADERS += MainWindow.h
FORMS   += MainWindow.ui

#-------------------------------------------------
#
# Merkopolo
#
#-------------------------------------------------

MERKOPOLO_SRC_DIR = $$PWD

include($$MERKOPOLO_SRC_DIR/mpInteractions/mpInteractions.pri)
include($$MERKOPOLO_SRC_DIR/mpWidgets/mpWidgets.pri)
include($$MERKOPOLO_SRC_DIR/mpLayers/mpLayers.pri)

TARGET = merkopolo
INSTALLS += target
TEMPLATE = app

SOURCES += main.cpp\
           mpwindow.cpp

HEADERS  += mpglobal.h \
            mpwindow.h

FORMS    += mpwindow.ui

RESOURCES += $$MERKOPOLO_SRC_DIR/resources/icons/icons.qrc

OTHER_FILES += ../README

TRANSLATIONS = $$MERKOPOLO_SRC_DIR/locale/merkopolo-fr.ts

win32: RC_FILE += $$MERKOPOLO_SRC_DIR/resources/merkopolo.rc
