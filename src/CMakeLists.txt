set ( SOURCES
	dialogs/mainDialog.cpp
	dialogs/mainDialog.h
	dialogs/pbcCreateMotionRouteDialog.cpp
	dialogs/pbcCreateMotionRouteDialog.h
	dialogs/pbcCustomRouteDialog.cpp
	dialogs/pbcCustomRouteDialog.h
	dialogs/pbcEditCategoriesDialog.cpp
	dialogs/pbcEditCategoriesDialog.h
	dialogs/pbcExportPdfDialog.cpp
	dialogs/pbcExportPdfDialog.h
	dialogs/pbcNewPlaybookDialog.cpp
	dialogs/pbcNewPlaybookDialog.h
	dialogs/pbcNewPlayDialog.cpp
	dialogs/pbcNewPlayDialog.h
	dialogs/pbcOpenPlayDialog.cpp
	dialogs/pbcOpenPlayDialog.h
	dialogs/pbcSavePlayAsDialog.cpp
	dialogs/pbcSavePlayAsDialog.h
	gui/pbcCustomRouteView.cpp
	gui/pbcCustomRouteView.h
	gui/pbcGridIronView.cpp
	gui/pbcGridIronView.h
	gui/pbcPlayerView.cpp
	gui/pbcPlayerView.h
	gui/pbcPlayView.cpp
	gui/pbcPlayView.h
	models/pbcAbstractMovement.h
	models/pbcCategory.cpp
	models/pbcCategory.h
	models/pbcColor.cpp
	models/pbcColor.h
	models/pbcFormation.h
	models/pbcMotion.cpp
	models/pbcMotion.h
	models/pbcPath.cpp
	models/pbcPath.h
	models/pbcPlay.cpp
	models/pbcPlay.h
	models/pbcPlaybook.cpp
	models/pbcPlaybook.h
	models/pbcPlayer.cpp
	models/pbcPlayer.h
	models/pbcRoute.cpp
	models/pbcRoute.h
	util/pbcConfig.h
	util/pbcDeclarations.h
	util/pbcExceptions.h
	util/pbcPositionTranslator.cpp
	util/pbcPositionTranslator.h
	util/pbcSingleton.h
	util/pbcStorage.cpp
	util/pbcStorage.h
	main.cpp
	pbcVersion.h
)

set ( MOC_HEADERS
	pbcVersion.h
	dialogs/mainDialog.h
	util/pbcDeclarations.h
	models/pbcPlay.h
	models/pbcPlayer.h
	models/pbcRoute.h
	models/pbcPath.h
	util/pbcConfig.h
	models/pbcCategory.h
	models/pbcFormation.h
	util/pbcSingleton.h
	gui/pbcPlayerView.h
	util/pbcPositionTranslator.h
	dialogs/pbcCustomRouteDialog.h
	gui/pbcCustomRouteView.h
	gui/pbcPlayView.h
	gui/pbcGridIronView.h
	models/pbcPlaybook.h
	models/pbcColor.h
	util/pbcStorage.h
	util/pbcExceptions.h
	dialogs/pbcExportPdfDialog.h
	models/pbcMotion.h
	dialogs/pbcCreateMotionRouteDialog.h
	models/pbcAbstractMovement.h
	dialogs/pbcNewPlaybookDialog.h
	dialogs/pbcNewPlayDialog.h
	dialogs/pbcOpenPlayDialog.h
	dialogs/pbcEditCategoriesDialog.h
	dialogs/pbcSavePlayAsDialog.h
)

set ( UIS
	dialogs/mainDialog.ui
    dialogs/pbcCustomRouteDialog.ui
    dialogs/pbcExportPdfDialog.ui
    dialogs/pbcCreateMotionRouteDialog.ui
    dialogs/pbcNewPlaybookDialog.ui
    dialogs/pbcNewPlayDialog.ui
    dialogs/pbcOpenPlayDialog.ui
    dialogs/pbcEditCategoriesDialog.ui
    dialogs/pbcSavePlayAsDialog.ui
)

FIND_PACKAGE( Boost 1.54 COMPONENTS serialization REQUIRED )

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src ${Boost_INCLUDE_DIR})

link_directories(${PROJECT_SOURCE_DIR}/lib)
set ( LIBS
	#libboost_serialization.a
	libbotan-1.10.a
)

# Next lines needed for building all Qt projects
find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )

# Next, using precompiler, compiler and linker
# using Qt meta-system (precompiler)
QT4_WRAP_UI( UI_HEADERS ${UIS} )
QT4_WRAP_CPP( MOC_SRCS ${MOC_HEADERS} )

# compile
add_executable( ${PROJECT_NAME} ${SOURCES} ${MOC_SRCS} ${UI_HEADERS} )

# build it (link libraries)
target_link_libraries( ${PROJECT_NAME} ${LIBS} ${QT_LIBRARIES} ${Boost_LIBRARIES})