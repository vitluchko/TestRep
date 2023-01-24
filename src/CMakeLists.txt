set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

find_package(imgui CONFIG)
find_package(glfw3 CONFIG)
find_package(glew CONFIG)

add_executable(ServerUI
	ServerUI.cpp
	ServerUI.h
	../bindings/imgui_impl_glfw.cpp
	../bindings/imgui_impl_opengl3.cpp
	../bindings/imgui_impl_glfw.h
	../bindings/imgui_impl_opengl3.h)

message("********** CONAN_LIBS: " "${CONAN_LIBS}"
 ********)

#target_compile_definitions(ServerUI PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_link_libraries(ServerUI imgui::imgui GLEW::glew_s glfw::glfw)
	
#target_link_libraries(ServerUI ${CONAN_LIBS} imgui_impl_opengl3 imgui_impl_glfw)