
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

add_executable(ServerUI ServerUI.cpp ServerUI.h)
	
target_link_libraries(ServerUI ${CONAN_LIBS} imgui_impl_opengl3 imgui_impl_glfw )