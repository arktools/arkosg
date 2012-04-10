project (arkosg CXX C)

# settings
cmake_minimum_required (VERSION 2.8)
set(PROJECT_VERSION_MAJOR "0")
set(PROJECT_VERSION_MINOR "3")
set(PROJECT_VERSION_PATCH "0")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(PROJECT_CONTACT_EMAIL james.goppert@gmail.com)
set(PROJECT_CONTACT_VENDOR arkTools)
set(LIBRARY_VERSION ${PROJECT_VERSION})
set(LIBRARY_SOVERSION "0.1.0")

# third party libraries
set(BOOST_REQUIRED_VERSION "1.42.0")
set(QT_REQUIRED_VERSION "4.6.3")

# options
option(WITH_BUILD_DEPS "Build dependencies." OFF)
option(WITH_PREFER_STATIC "Build preferring static linking." ON)
option(WITH_BUILD_TESTS "Build a test application with QT" OFF)
option(WITH_BUILD_SHARED "Build shared library." OFF)
option(WITH_BUNDLE "Build executables." OFF)
option(WITH_WARNINGS "Enable warnings." OFF)

# variables
set(ROOT_THREAD TRUE CACHE INTERNAL "Is this the top level of the recursion?")

# modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_SOURCE_DIR}/cmake/arkcmake)
include(DefineCMakeDefaults)
include(DefineCompilerFlags)
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckLibraryExists)
#include(CheckTypeSize)
#include(CheckPrototypeExists)
#include(CheckCXXSourceCompiles)
#include(CheckCSourceCompiles)
include(ExternalProjectWithFilename)

# spawn new cmake to build deps
if (WITH_BUILD_DEPS AND ROOT_THREAD)
    execute_process(COMMAND ${CMAKE_COMMAND} "${CMAKE_SOURCE_DIR}"
        "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
        "-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}"
        "-DEP_BASE_DIR=${EP_BASE_DIR}"
        "-DWITH_BUILD_DEPS=${WITH_BUILD_DEPS}"
        "-DWITH_PREFER_STATIC=${WITH_PREFER_STATIC}"
        "-DROOT_THREAD=FALSE"
        RESULT_VARIABLE ERROR)
    if (ERROR)
        message(FATAL_ERROR "error, recursing loop returned error code: ${ERROR}")
    endif()
    message("** Making dependencies")
    execute_process(COMMAND ${CMAKE_MAKE_PROGRAM} "-j4" "-f${CMAKE_BINARY_DIR}/Makefile")
    message("** Configuring ${PROJECT_NAME}")
endif()

# external projects find path
if(NOT EP_BASE_DIR)
    set(EP_BASE_DIR "${CMAKE_BINARY_DIR}/CMakeExternals")
endif()
list(APPEND CMAKE_FIND_ROOT_PATH ${EP_BASE_DIR})

# prefer static packages if building static library
message("** Finding libraries")

# static libraries
if (WITH_PREFER_STATIC)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME ON)
    # prefer static libs
    if(WIN32)
        set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
    else()
        set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
    endif()
endif()

# find libraries with cmake modules
find_package(Qt4 ${QT_REQUIRED_VERSION} REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenSceneGraph ${OPENSCENEGRAPH_REQUIRED_VERSION} REQUIRED COMPONENTS osgViewer osgUtil osgDB osgGA osgManipulator)
if(NOT MSVC)
    find_package(OsgPlugin ${OPENSCENEGRAPH_REQUIRED_VERSION} ${REQUIRED_IF_ROOT_THREAD} COMPONENTS ac rgb)
endif()

if(MSVC)
    set(Boost_USE_MULTITHREADED ON)
    find_package(Boost ${BOOST_REQUIRED_VERSION} REQUIRED COMPONENTS thread system)
elseif(MINGW)
    find_package(Boost ${BOOST_REQUIRED_VERSION} REQUIRED COMPONENTS thread_win32-mt system-mt)
else()
    find_package(Boost ${BOOST_REQUIRED_VERSION} REQUIRED COMPONENTS thread-mt system-mt)
endif()

# build dependencies
if (WITH_BUILD_DEPS AND (NOT ROOT_THREAD) )
    message("** Configuring dependencies")

    # add external projects

    set(CMAKE_DEFAULT_ARGS
        -DEP_BASE_DIR=${EP_BASE_DIR}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        )

    # none currently

    # terminate non root cmake thread
    return()
endif()

# configure
#check_include_files(string.h HAVE_STRING_H)
#check_function_exists(memcopy HAVE_MEMCOPY)
#check_symbol_exists(LC_MESSAGES "locale.h" HAVE_LC_MESSAGES) 
#check_library_exists(arkosg  attachNode "" HAVE_ARKOSG)

# config files
set(INSTALL_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}")
configure_file(config.h.in config.h)
install(FILES ${CMAKE_BINARY_DIR}/config.h DESTINATION include/${PROJECT_NAME} COMPONENT Dev)

# build settings
set(QT_USE_QTOPENGL TRUE)
add_definitions(-DBOOST_THREAD_USE_LIB)
if (WITH_PREFER_STATIC OR MINGW)
    add_definitions(-DOSG_LIBRARY_STATIC)
endif()
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Build architectures for OSX" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS "-framework cocoa -framework carbon")
endif()
if (MSVC)
    add_definitions( -D_USE_MATH_DEFINES)
endif()
include( ${QT_USE_FILE})
add_definitions(-DUSE_QT4)

if (WITH_WARNINGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -pedantic")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wshadow -Wmissing-prototypes -Wdeclaration-after-statement")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wformat-security")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-format-attribute")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wformat-security")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-format-attribute")
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
    ${OPENSCENEGRAPH_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${QT_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    )
link_directories(
    ${OPENSCENEGRAPH_LIBRARY_DIRS}
    ${QT_LIBRARY_DIR}
    ) 

# install data files
install(DIRECTORY "data/images" DESTINATION share/${PROJECT_NAME} COMPONENT Runtime PATTERN "*.git*" EXCLUDE)
install(DIRECTORY "data/models" DESTINATION share/${PROJECT_NAME} COMPONENT Runtime PATTERN "*.git*" EXCLUDE)

# arkosg library
set(ARKOSG_SRCS
    src/osgUtils.cpp
    src/Viewer.cpp
    src/terrain.cpp
    )
set(ARKOSG_HDRS
    src/osgUtils.hpp
    src/Viewer.hpp
    src/terrain_coords.h
    src/terrain_normals.h
    src/terrain_texcoords.h
    )
if(QT_FOUND)
    list(APPEND ARKOSG_SRCS
        src/QOSGAdapterWidget.cpp
    )
    list(APPEND ARKOSG_HDRS
        src/QOSGAdapterWidget.hpp
    )
endif()

# install headers
install(FILES ${ARKOSG_HDRS} DESTINATION include/arkosg COMPONENT Dev)

# link libraries
set(ARKOSG_LINK_LIBRARIES
    ${Boost_LIBRARIES}
    ${OSGPLUGIN_LIBRARIES}
    ${OPENSCENEGRAPH_LIBRARIES} 
    ${OPENGL_LIBRARIES}
    ${QT_LIBRARIES}
    )
if(MINGW)
    list(APPEND ARKOSG_LINK_LIBRARIES 
        lcms
        lzma
    )       
endif()

# static library
add_library(arkosgStatic STATIC ${ARKOSG_SRCS} ${ARKOSG_HDRS})
set(ARKOSG_STATIC_LIBNAME "arkosg")
if (MSVC)
    set(ARKOSG_STATIC_LIBNAME "arkosgStatic")
endif()
set_target_properties(arkosgStatic PROPERTIES 
    OUTPUT_NAME ${ARKOSG_STATIC_LIBNAME}
    VERSION ${LIBRARY_VERSION} 
    )
install(TARGETS arkosgStatic 
    ARCHIVE DESTINATION "lib" COMPONENT Runtime
    )

if (MSVC)
     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /FORCE:Multiple")
# for some strange reason /EHsc needs to be set twice
     SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
     SET_TARGET_PROPERTIES(arkosgStatic PROPERTIES COMPILER_FLAGS "/EHsc")

 endif()
 
# shared library
if (WITH_BUILD_SHARED)
    add_library(arkosgShared SHARED ${ARKOSG_SRCS} ${ARKOSG_HDRS})
    set_target_properties(arkosgShared PROPERTIES 
        OUTPUT_NAME arkosg
        VERSION ${LIBRARY_VERSION} 
        SOVERSION ${LIBRARY_SOVERSION}
        FRAMEWORK ON
        )
    target_link_libraries(arkosgShared ${ARKOSG_LINK_LIBRARIES})
    install(TARGETS arkosgShared
        LIBRARY DESTINATION "lib" COMPONENT Runtime
        FRAMEWORK DESTINATION "/Library/Frameworks" COMPONENT Runtime
        # where to put files on non-mac computers, mac overrides and uses default
        PRIVATE_HEADER DESTINATION "include/arkosg" COMPONENT Runtime
        PUBLIC_HEADER DESTINATION "include/arkosg" COMPONENT Runtime
        RESOURCE DESTINATION "share/arkosg" COMPONENT Runtime
        )
endif()

# test
if (WITH_BUILD_TESTS)
    set(ARKOSG_TEST_MOC_SCRS
        test/MainWindow.hpp
        )
    qt4_wrap_cpp(ARKOSG_TEST_MOC ${ARKOSG_TEST_MOC_SCRS})
    set(ARKOSG_TEST_UI_SRCS
        test/MainWindow.ui
        )
    qt4_wrap_ui(ARKOSG_TEST_UI ${ARKOSG_TEST_UI_SRCS})
    set(ARKOSG_TEST_RES_SRCS
        )
    qt4_add_resources(ARKOSG_TEST_RES ${ARKOSG_TEST_RES_SRCS})
    set(ARKOSG_TEST_SRC
        test/MainWindow.cpp
        test/main.cpp
        ${ARKOSG_TEST_MOC}
        ${ARKOSG_TEST_UI}
        ${ARKOSG_TEST_RES}
        )
    if (MSVC)
        add_executable(arkosgTest ${ARKOSG_TEST_SRC})
    else()
        add_executable(arkosgTest WIN32 MACOSX_BUNDLE ${ARKOSG_TEST_SRC})
    endif()
    target_link_libraries(arkosgTest arkosgStatic ${ARKOSG_LINK_LIBRARIES})
    install(TARGETS arkosgTest
        RUNTIME DESTINATION "bin" COMPONENT Runtime
        BUNDLE DESTINATION "/Applications" COMPONENT Runtime
        )
    if(APPLE)
        add_custom_command(TARGET arkosgTest POST_BUILD COMMAND macdeployqt ${CMAKE_BINARY_DIR}/arkosgTest.app)
        add_custom_command(TARGET arkosgTest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory /opt/local/lib/Resources/qt_menu.nib ${CMAKE_BINARY_DIR}/arkosgTest.app/Contents/Resources/qt_menu.nib)
    endif()
endif()

### packaging

# bundle dependencies
if (WITH_BUILD_TESTS AND WITH_BUNDLE)
    set(APPS "\$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/arkosgTest")  # paths to executables
    INSTALL(CODE "
        set(DIRS \"${QT_LIBRARY_DIR}\") # directories to search for prerequisites
        message(STATUS \"DIRS: \${DIRS}\")
        file(GLOB_RECURSE QTPLUGINS
            \"${QT_PLUGINS_DIR}/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
        include(BundleUtilities)
        fixup_bundle(\"${APPS}\"   \"\${QTPLUGINS}\"   \"\${DIRS}\")
        " COMPONENT Runtime)
endif()

# set icns file containing icons
if (APPLE)
    # set how it shows up in Info.plist
    set(MACOSX_BUNDLE_ICON_FILE arkosg.icns)
    # set where in the bundle to put the icns file
    set_source_files_properties(cmake/arkosg.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    # include the icns file in the target
    list(APPEND ARKOSG_TEST_SRCS cmake/arkosg.icns)
endif(APPLE)

# set NSIS image
if (WIN32)
    # nsis bug requuires atleast one file with 4 slashes to install correctly
    set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\/cmake/arkosg-nsis.bmp")
endif(WIN32)

# add file extensions and set resource files
configure_file("COPYING" "COPYING.txt" COPYONLY)
configure_file("README" "README.txt" COPYONLY)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_BINARY_DIR}/COPYING.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_BINARY_DIR}/README.txt")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CPACK_RESOURCE_FILE_README}")
set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/cmake/WELCOME.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "arktools osg library")
set(CPACK_PACKAGE_VENDOR ${PROJECT_CONTACT_VENDOR})
set(CPACK_PACKAGE_CONTACT "${PROJECT_CONTACT_EMAIL}")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_SET_DESTDIR TRUE)
set(CPACK_COMPONENTS_GROUPING "ALL_COMPONENTS_IN_ONE")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++6, libgcc1, libc6, libgl1-mesa-glx")
if (WITH_BUILD_TESTS)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, libqtcore4, libqtgui4, libxcb1, libqt4-opengl")
endif()
set(CPACK_PACKAGE_EXECUTABLES
    #"cmd" "Command Line"
    #"gui" "Gui"
    )
if (WITH_BUILD_TESTS)
    list(APPEND CPACK_PACKAGE_EXECUTABLES "arkosgTest" "test")
endif()
include(CPack)

# pkgconfig
configure_file(pc.in ${PROJECT_NAME}.pc)
install(FILES
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION "lib/pkgconfig" COMPONENT Dev
    )
# vim:sw=4:ts=4:expandtab
