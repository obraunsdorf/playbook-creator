set ( SOURCES
	dialogs/mainDialog.cpp
	dialogs/mainDialog.h
	dialogs/pbcCustomRouteDialog.cpp
	dialogs/pbcCustomRouteDialog.h
	dialogs/pbcDeleteDialog.cpp
	dialogs/pbcDeleteDialog.h
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
	dialogs/pbcSetPasswordDialog.cpp
	dialogs/pbcSetPasswordDialog.h
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
	models/pbcDefaultPlaybook.cpp
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
	pbcVersion.h)

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
	dialogs/pbcDeleteDialog.h
	models/pbcMotion.h
    models/pbcAbstractMovement.h
	dialogs/pbcNewPlaybookDialog.h
	dialogs/pbcNewPlayDialog.h
	dialogs/pbcOpenPlayDialog.h
	dialogs/pbcEditCategoriesDialog.h
	dialogs/pbcSavePlayAsDialog.h
	dialogs/pbcSetPasswordDialog.h
)

set ( UIS
	dialogs/mainDialog.ui
    dialogs/pbcCustomRouteDialog.ui
	dialogs/pbcDeleteDialog.ui
    dialogs/pbcExportPdfDialog.ui
	dialogs/pbcNewPlaybookDialog.ui
    dialogs/pbcNewPlayDialog.ui
    dialogs/pbcOpenPlayDialog.ui
    dialogs/pbcEditCategoriesDialog.ui
    dialogs/pbcSavePlayAsDialog.ui
	dialogs/pbcSetPasswordDialog.ui
)

include_directories(${PROJECT_SOURCE_DIR}/src})

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package( Boost 1.54 COMPONENTS serialization REQUIRED )
add_definitions( -DBOOST_ALL_NO_LIB )
include_directories(${Boost_INCLUDE_DIR})

find_package( Botan 1.10 REQUIRED )
include_directories(${BOTAN_INCLUDE_DIR})

# Next lines needed for building all Qt projects
find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5PrintSupport_INCLUDE_DIRS})

add_definitions(${Qt5Widgets_DEFINITIONS})
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
#set(CMAKE_AUTOMOC ON)

# Next, using precompiler, compiler and linker
# using Qt meta-system (precompiler)
qt5_wrap_ui( UI_HEADERS ${UIS} )
qt5_wrap_cpp( MOC_SRCS ${MOC_HEADERS} )

# linting
#set (STYLE_CHECKER ${PROJECT_SOURCE_DIR}/tools/checkStyle.py)
#find_package(PythonInterp 3.0 REQUIRED)
#add_custom_target(style_checking
#	COMMAND ${PYTHON_EXECUTABLE} ${STYLE_CHECKER}
#	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tools
#)

# compile
add_executable( ${PROJECT_NAME} ${SOURCES} ${MOC_SRCS} ${UI_HEADERS})
if(APPLE)
	set_target_properties(${PROJECT_NAME} PROPERTIES
	        MACOSX_BUNDLE ON
	        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/pbcMacOSXBundleInfo.plist.in)
endif()
#add_dependencies(${PROJECT_NAME} style_checking)

# link Microsoft Visual Studio Runtime statically by default (on Windows builds)
include(ConfigureMSVCRT)
configure_msvc_runtime()

# link libraries
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::PrintSupport ${Boost_LIBRARIES} ${BOTAN_LIBRARY})
#target_link_libraries(${PROJECT_NAME} ${Qt5Widgets_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} ${Boost_LIBRARIES} ${BOTAN_LIBRARY})
