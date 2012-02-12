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

set(arkosgTest_src
	MainWindow.cpp
	main.cpp
	${arkosgTest_moc}
	${arkosgTest_ui}
	${arkosgTest_res}
	)

add_executable(arkosgTest ${arkosgTest_src})
add_test(arkosgTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/arkosgTest)
target_link_libraries(arkosgTest arkosg)

install(TARGETS arkosgTest DESTINATION bin)
