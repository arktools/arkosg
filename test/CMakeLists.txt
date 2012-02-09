include( ${QT_USE_FILE})
add_definitions(-DUSE_QT4)
link_directories(${OPENSCENEGRAPH_LIBRARY_DIRS} ${QT_LIBRARY_DIRS}) 
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
link_libraries(${OPENSCENEGRAPH_LIBRARIES} 
	${Boost_LIBRARIES} ${QT_LIBRARIES} ${OPENGL_LIBRARIES})

set(qtTest_moc_src
	MainWindow.hpp
	)

set(qtTest_ui_src
	MainWindow.ui
	)

set(qtTest_res
	)

qt4_wrap_cpp(qtTest_moc ${qtTest_moc_src})
qt4_wrap_ui(qtTest_ui ${qtTest_ui_src})
qt4_add_resources(qtTest_res ${qtTest_res_src})

include_directories(. ${OPENSCENEGRAPH_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

set(qtTest_src
	MainWindow.cpp
	main.cpp
	${qtTest_moc}
	${qtTest_ui}
	${qtTest_res}
	)

add_definitions(-DUSE_QT4)
add_executable(qtTest ${qtTest_src})
add_test(qtTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qtTest)
target_link_libraries(qtTest ${OPENSCENEGRAPH_LIBRARIES} arkosg)
