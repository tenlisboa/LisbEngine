cmake_minimum_required(VERSION 3.27)
project(lisbengine)

set(CMAKE_CXX_STANDARD 17)
set(GLFW_PATH ${CMAKE_SOURCE_DIR}/libs/glfw/libglfw.3.4.dylib)

include_directories(./include)

add_executable(lisbengine src/main.cpp libs/glad.cpp)

set(link_src "${CMAKE_SOURCE_DIR}/assets")
set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/assets")
add_custom_command(
        TARGET lisbengine
        POST_BUILD COMMAND ${CMAKE_COMMAND}
        -E create_symlink ${link_src} ${link_dst}
        DEPENDS ${link_dst}
        COMMENT "symbolic link ${link_src} => ${link_dst}"
)

target_link_libraries(lisbengine PRIVATE ${GLFW_PATH})

