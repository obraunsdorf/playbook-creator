#-------------------------------------------------
#
# Project created by QtCreator 2015-02-06T00:09:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlaybookCreator
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainDialog.cpp \
    models/pbcPlay.cpp \
    models/pbcPlayer.cpp \
    models/pbcRoute.cpp \
    models/pbcPath.cpp \
    gui/pbcGridIronView.cpp \
    gui/pbcPlayerView.cpp \
    util/pbcPositionTranslator.cpp \
    dialogs/pbcCustomRouteDialog.cpp \
    gui/pbcCustomRouteView.cpp

HEADERS  += mainDialog.h \
    util/pbcDeclarations.h \
    models/pbcPlay.h \
    models/pbcPlayer.h \
    models/pbcRoute.h \
    models/pbcPath.h \
    util/pbcConfig.h \
    models/pbcCategory.h \
    models/pbcFormation.h \
    gui/pbcGridIronView.h \
    util/pbcSingleton.h \
    gui/pbcPlayerView.h \
    util/pbcPositionTranslator.h \
    dialogs/pbcCustomRouteDialog.h \
    gui/pbcCustomRouteView.h

FORMS    += mainDialog.ui \
    dialogs/pbcCustomRouteDialog.ui
