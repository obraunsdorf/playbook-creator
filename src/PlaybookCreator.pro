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

LIBS += -L/usr/lib/x86_64-linux-gnu -lboost_serialization

SOURCES += main.cpp\
    dialogs/mainDialog.cpp \
    models/pbcPlay.cpp \
    models/pbcPlayer.cpp \
    models/pbcRoute.cpp \
    models/pbcPath.cpp \
    gui/pbcPlayerView.cpp \
    util/pbcPositionTranslator.cpp \
    dialogs/pbcCustomRouteDialog.cpp \
    gui/pbcCustomRouteView.cpp \
    gui/pbcPlayView.cpp \
    gui/pbcGridIronView.cpp \
    models/pbcPlaybook.cpp \
    models/pbcDataModel.cpp \
    models/pbcColor.cpp

HEADERS  += dialogs/mainDialog.h \
    util/pbcDeclarations.h \
    models/pbcPlay.h \
    models/pbcPlayer.h \
    models/pbcRoute.h \
    models/pbcPath.h \
    util/pbcConfig.h \
    models/pbcCategory.h \
    models/pbcFormation.h \
    util/pbcSingleton.h \
    gui/pbcPlayerView.h \
    util/pbcPositionTranslator.h \
    dialogs/pbcCustomRouteDialog.h \
    gui/pbcCustomRouteView.h \
    gui/pbcPlayView.h \
    gui/pbcGridIronView.h \
    models/pbcPlaybook.h \
    models/pbcDataModel.h \
    models/pbcColor.h

FORMS    += dialogs/mainDialog.ui \
    dialogs/pbcCustomRouteDialog.ui
