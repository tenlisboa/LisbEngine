cmake_minimum_required(VERSION 3.27)
project(openglbook)

set(CMAKE_CXX_STANDARD 17)
set(GLFW_PATH ${CMAKE_SOURCE_DIR}/libs/glfw/3.4/lib/libglfw.3.4.dylib)

include_directories(./include)
include_directories(./libs/glfw/3.4/include ./libs/glad/include ./libs/KHR/include ./libs/stb_image/include)

add_executable(openglbook src/main.cpp libs/glad/src/glad.cpp
        include/core/shader.h
        include/core/texture.h
        libs/stb_image/include/stb_image/stb_image.h
)


set(link_src "${CMAKE_SOURCE_DIR}/assets")
set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/assets")
add_custom_command(
        TARGET openglbook
        POST_BUILD COMMAND ${CMAKE_COMMAND}
        -E create_symlink ${link_src} ${link_dst}
        DEPENDS ${link_dst}
        COMMENT "symbolic link ${link_src} => ${link_dst}"
)

target_link_libraries(openglbook PRIVATE ${GLFW_PATH})

