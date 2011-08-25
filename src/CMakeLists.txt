include( ${QT_USE_FILE})
add_definitions(-DUSE_QT4)
link_directories(${OPENSCENEGRAPH_LIBRARY_DIRS} ${QT_LIBRARY_DIRS}) 
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
link_libraries(${OPENSCENEGRAPH_LIBRARIES} 
	${Boost_LIBRARIES} ${QT_LIBRARIES} ${OPENGL_LIBRARIES})


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(arkOsgSource
	osgUtils.cpp
	Viewer.cpp
	)

set(arkOsgHeaders
	osgUtils.hpp
	Viewer.hpp
	)

if(QT_FOUND)
	list(APPEND arkOsgSource
		QOSGAdapterWidget.cpp
	)
	list(APPEND arkOsgHeaders
		QOSGAdapterWidget.hpp
	)
endif(QT_FOUND)

add_library(arkOsg SHARED ${arkOsgSource})

set_target_properties(arkOsg PROPERTIES VERSION ${LIBRARY_VERSION}
	SOVERSION ${LIBRARY_SOVERSION})

install(TARGETS arkOsg DESTINATION lib)
install(FILES ${arkOsgHeaders} DESTINATION include/arkOsg)
