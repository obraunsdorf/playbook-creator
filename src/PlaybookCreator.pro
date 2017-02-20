########################
##    This file is part of Playbook Creator.
##
##    Playbook Creator is free software: you can redistribute it and/or modify
##    it under the terms of the GNU General Public License as published by
##    the Free Software Foundation, either version 3 of the License, or
##    (at your option) any later version.
##
##    Playbook Creator is distributed in the hope that it will be useful,
##    but WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##    GNU General Public License for more details.
##
##    You should have received a copy of the GNU General Public License
##    along with Playbook Creator.  If not, see <http://www.gnu.org/licenses/>.
##
##    Copyright 2015 Oliver Braunsdorf
##
##    @author Oliver Braunsdorf
########################

#-------------------------------------------------
#
# Project created by QtCreator 2015-02-06T00:09:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlaybookCreator
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -Wno-unused-local-typedefs

INCLUDEPATH += ../include/

LIBS += ../lib/libbotan-1.10.a \
    ../lib/libboost_serialization.a

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
    models/pbcColor.cpp \
    util/pbcStorage.cpp \
    dialogs/pbcExportPdfDialog.cpp \
    models/pbcMotion.cpp \
    dialogs/pbcCreateMotionRouteDialog.cpp \
    models/pbcCategory.cpp \
    dialogs/pbcNewPlaybookDialog.cpp \
    dialogs/pbcNewPlayDialog.cpp \
    dialogs/pbcOpenPlayDialog.cpp \
    dialogs/pbcEditCategoriesDialog.cpp \
    dialogs/pbcSavePlayAsDialog.cpp

HEADERS  += pbcVersion.h \
    dialogs/mainDialog.h \
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
    models/pbcColor.h \
    util/pbcStorage.h \
    util/pbcExceptions.h \
    dialogs/pbcExportPdfDialog.h \
    models/pbcMotion.h \
    dialogs/pbcCreateMotionRouteDialog.h \
    models/pbcAbstractMovement.h \
    dialogs/pbcNewPlaybookDialog.h \
    dialogs/pbcNewPlayDialog.h \
    dialogs/pbcOpenPlayDialog.h \
    dialogs/pbcEditCategoriesDialog.h \
    dialogs/pbcSavePlayAsDialog.h

FORMS    += dialogs/mainDialog.ui \
    dialogs/pbcCustomRouteDialog.ui \
    dialogs/pbcExportPdfDialog.ui \
    dialogs/pbcCreateMotionRouteDialog.ui \
    dialogs/pbcNewPlaybookDialog.ui \
    dialogs/pbcNewPlayDialog.ui \
    dialogs/pbcOpenPlayDialog.ui \
    dialogs/pbcEditCategoriesDialog.ui \
    dialogs/pbcSavePlayAsDialog.ui
