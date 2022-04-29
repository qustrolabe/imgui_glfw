cmake_minimum_required(VERSION 3.20.0)
project(imgui-test VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)


# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})


include(cmake/ImGui.cmake)


add_executable(${PROJECT_NAME} 
	main.cpp
)

target_link_libraries(${PROJECT_NAME}
	ImGui
	OpenGL
	# GLEW
	glfw
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${imgui_SOURCE_DIR}
	${imgui_SOURCE_DIR}/backends
)
