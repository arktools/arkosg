link_directories(
    ${OPENSCENEGRAPH_LIBRARY_DIRS}
    ${QT_LIBRARY_DIRS}
    ) 

include_directories(
    ${OPENSCENEGRAPH_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(${PROJECT_NAME}Source
	osgUtils.cpp
	Viewer.cpp
	terrain.cpp
	)

set(${PROJECT_NAME}Headers
	osgUtils.hpp
	Viewer.hpp
	terrain_coords.h
	terrain_normals.h
	terrain_texcoords.h
	)

if(QT_FOUND)
	list(APPEND ${PROJECT_NAME}Source
		QOSGAdapterWidget.cpp
	)
	list(APPEND ${PROJECT_NAME}Headers
		QOSGAdapterWidget.hpp
	)
endif(QT_FOUND)

add_library(${PROJECT_NAME} ${${PROJECT_NAME}Source})

set(${PROJECT_NAME}LinkLibs
    ${OPENSCENEGRAPH_LIBRARIES} 
    ${QT_LIBRARIES}
    )

if (WIN32)
    if (NOT BUILD_SHARED_LIBS)
        list(APPEND ${PROJECT_NAME}LinkLibs
            lcms
            lzma
        )
    endif()
endif()

target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}LinkLibs})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${LIBRARY_VERSION}
	SOVERSION ${LIBRARY_SOVERSION}
    )

install(TARGETS ${PROJECT_NAME} DESTINATION lib EXPORT ${PROJECT_NAME}-targets)
install(EXPORT ${PROJECT_NAME}-targets DESTINATION lib/${PROJECT_NAME})
install(FILES ${${PROJECT_NAME}Headers} DESTINATION include/${PROJECT_NAME})
