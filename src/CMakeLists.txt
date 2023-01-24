set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)


set(SOURCE_FILES
	ServerUI.cpp
	ServerUI.h
	../bindings/imgui_impl_glfw.cpp
	../bindings/imgui_impl_opengl3.cpp
	../bindings/imgui_impl_glfw.h
	../bindings/imgui_impl_opengl3.h)

message("********** CONAN_LIBS: " "${CONAN_LIBS}")


add_executable(ServerUI ${SOURCE_FILES})
target_link_libraries(ServerUI ${CONAN_LIBS})
	
