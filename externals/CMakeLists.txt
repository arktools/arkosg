project (arkosg-externals NONE)

# settings
cmake_minimum_required (VERSION 2.6)
set(APPLICATION_NAME ${PROJECT_NAME})
set(APPLICATION_VERSION_MAJOR "0")
set(APPLICATION_VERSION_MINOR "2")
set(APPLICATION_VERSION_PATCH "1")
set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")
set(LIBRARY_SOVERSION "0.0.0")
set(OSGEARTH_VERSION "664515eed414ad1f18e85575913843c41e71026d")
set(PROJECT_CONTACT_EMAIL james.goppert@gmail.com)
set(PROJECT_CONTACT_VENDOR arkTools)

# modules
set(CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/cmake
	${CMAKE_SOURCE_DIR}/cmake/arkcmake
)
include(FindOSGEARTH)
include(BuildOpenSceneGraph)

# use common setup
include(CPackPackage)
include(CommonSetup)
set(LIBRARY_VERSION ${APPLICATION_VERSION})

# settings
option(WITH_BUILD_STATIC "Prefer static linking." ON)
option(WITH_OSGEARTH "With osgearth" OFF)
option(WITH_QT "With QT" ON)
if (WITH_QT)
    option(BUILD_TEST "Build a test application with QT" OFF)
endif()

# static lib options
if (WITH_BUILD_STATIC)
    add_definitions(-DOSG_LIBRARY_STATIC)
    set(BOOST_USE_STATIC_LIBS ON)
    if(WIN32)
        set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
    else()
        set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
    endif()
endif()

# find libraries with cmake modules
if (WITH_QT)
    find_package(Qt4 COMPONENTS QtOpenGL QtGui QtCore REQUIRED)
    include( ${QT_USE_FILE})
    add_definitions(-DUSE_QT4)
endif()

if (WITH_OSGEARTH)
    find_package(OSGEARTH)
    if (NOT OSGEARTH_FOUND)
        build_osgearth(${OSGEARTH_VERSION} ${EP_BASE_DIR})
    endif()
endif()

if(WIN32)
    find_package(Boost 1.40.0 REQUIRED COMPONENTS thread_win32-mt system-mt)
else()
    find_package(Boost 1.40.0 REQUIRED COMPONENTS thread-mt system-mt)
endif()
find_package(OpenSceneGraph 3.0.1 COMPONENTS osgViewer osgUtil osgDB osgGA osgManipulator)
if (NOT OPENSCENEGRAPH_FOUND)
    message(STATUS "Building OpenSceneGraph from source.")
    set(CMAKE_ARGS_OSG
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        -DDYNAMIC_OPENSCENEGRAPH=n
        -DDYNAMIC_OPENTHREADS=n
        )
    build_openscenegraph(3.0.1 ${EP_BASE_DIR} "${CMAKE_ARGS_OSG}")
endif()

# install data files
install(DIRECTORY "${PROJECT_SOURCE_DIR}/data" DESTINATION share/${PROJECT_NAME} PATTERN "*.git*" EXCLUDE)

# pkgconfig
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc)
install(FILES
	${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc
	DESTINATION lib/pkgconfig
	)

# project wide flags
include_directories( ${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src)

# subdirectories
add_subdirectory(src)

if (WITH_QT AND WITH_BUILD_TEST)
	add_subdirectory(test)
endif ()
