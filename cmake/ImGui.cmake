cmake_minimum_required(VERSION 3.20.0)

include(cmake/CPM.cmake)

CPMAddPackage(
	GITHUB_REPOSITORY "ocornut/imgui"
	GIT_TAG "docking" # branch
	GIT_SHALLOW TRUE
)

add_library(
        ImGui STATIC
        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_internal.h

        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h

        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.h     
)

target_include_directories(
	ImGui PRIVATE
	${imgui_SOURCE_DIR}
	${imgui_SOURCE_DIR}/backends
)