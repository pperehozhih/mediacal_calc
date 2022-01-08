cmake_minimum_required(VERSION 3.13)
project(MediCalc)
set(CMAKE_CXX_STANDARD 17)
if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/scpm.cmake)
	file(DOWNLOAD https://raw.githubusercontent.com/pperehozhih/scpm/master/scpm.cmake ${CMAKE_CURRENT_BINARY_DIR}/scpm.cmake)
	#file(DOWNLOAD http://127.0.0.1:8000/scpm.cmake ${CMAKE_CURRENT_BINARY_DIR}/scpm.cmake)
endif()
include(${CMAKE_CURRENT_BINARY_DIR}/scpm.cmake)
set(scpm_server "http://127.0.0.1:8000/")
set(scpm_imgui_glfw_imgui_version docking)
set(scmp_build_type Debug)
scpm_install(imgui_glfw)
scpm_install(sqlite3)
include_directories(SYSTEM "${scpm_root_dir}/include")
link_directories("${scpm_root_dir}/lib")
set(RESOURCE
	res/res.rc)
scpm_create_target(TARGET ${PROJECT_NAME} TYPE GUI FILES
	src/main.cpp
	src/TextEditor.cpp
	src/TextEditor.h
	src/GeneralTable.cpp
	src/GeneralTable.h
	src/DataSource.cpp
	src/DataSource.h
	${RESOURCE})
scpm_link_target(TARGET ${PROJECT_NAME} LIBS imgui_glfw sqlite3)
configure_file(res/data.sl3 root/bin/data.sl3 COPYONLY)
scpm_debugger_setup(${PROJECT_NAME})

