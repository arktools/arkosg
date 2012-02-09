include( ${QT_USE_FILE})
add_definitions(-DUSE_QT4)
link_directories(${OPENSCENEGRAPH_LIBRARY_DIRS} ${QT_LIBRARY_DIRS}) 
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
link_libraries(${OPENSCENEGRAPH_LIBRARIES} 
	${Boost_LIBRARIES} ${QT_LIBRARIES} ${OPENGL_LIBRARIES})

set(arkosgTest_moc_src
	MainWindow.hpp
	)

set(arkosgTest_ui_src
	MainWindow.ui
	)

set(arkosgTest_res
	)

qt4_wrap_cpp(arkosgTest_moc ${arkosgTest_moc_src})
qt4_wrap_ui(arkosgTest_ui ${arkosgTest_ui_src})
qt4_add_resources(arkosgTest_res ${arkosgTest_res_src})

include_directories(. ${OPENSCENEGRAPH_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

set(arkosgTest_src
	MainWindow.cpp
	main.cpp
	${arkosgTest_moc}
	${arkosgTest_ui}
	${arkosgTest_res}
	)

add_definitions(-DUSE_QT4)
add_executable(arkosgTest ${arkosgTest_src})
add_test(arkosgTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/arkosgTest)
target_link_libraries(arkosgTest ${OPENSCENEGRAPH_LIBRARIES} arkosg)

install(TARGETS arkosgTest DESTINATION bin)
