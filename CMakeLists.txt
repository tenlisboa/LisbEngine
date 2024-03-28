cmake_minimum_required(VERSION 3.27)
project(lisbengine)

set(CMAKE_CXX_STANDARD 17)
# set(GLFW_PATH ${CMAKE_SOURCE_DIR}/libs/glfw/libglfw.3.4.dylib)

include_directories(./include)
include_directories(./external/glad/include)
include_directories(./external/stb_image/include)

add_executable(lisbengine src/main.cpp external/glad/src/glad.cpp)

include(FetchContent)

FetchContent_Declare(glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG "3.4"
    GIT_SHALLOW ON
)
FetchContent_Declare(glm
    GIT_REPOSITORY "https://github.com/g-truc/glm"
    GIT_TAG "1.0.1"
    GIT_SHALLOW ON
)

# Now make the glfw target available to link against
FetchContent_MakeAvailable(glfw glm)

target_link_libraries(lisbengine PRIVATE glfw glm)

set(link_src "${CMAKE_SOURCE_DIR}/assets")
set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/assets")
add_custom_command(
        TARGET lisbengine
        POST_BUILD COMMAND ${CMAKE_COMMAND}
        -E create_symlink ${link_src} ${link_dst}
        DEPENDS ${link_dst}
        COMMENT "symbolic link ${link_src} => ${link_dst}"
)

# target_link_libraries(lisbengine PRIVATE ${GLFW_PATH})

