project(learnOpenGL)

get_filename_component(PROJTCT_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
# # 假设你的项目目录是CMakeLists.txt所在目录的上一级
# get_filename_component(PROJTCT_PATH "${CURRENT_DIR}/.." ABSOLUTE)
message(STATUS "Project path set to: ${PROJTCT_PATH}")

# 使用 C++ 11 标准
set(CMAKE_CXX_STANDARD 11)

set(OPENGL_LIB_PATH ${PROJTCT_PATH}/lib-macos)
set(OPENGL_INCLUDE_PATH ${PROJTCT_PATH}/include)
set(LEARN_OPENGL_SOURCE_PATH ${PROJTCT_PATH}/src)

set(SDK_LIBS
    ${OPENGL_LIB_PATH}/arm64/libglfw.3.dylib
)

include_directories(
    ${OPENGL_INCLUDE_PATH}
    ${LEARN_OPENGL_SOURCE_PATH}
)

set(LEARN_OPENGL_SOURCE
    ${LEARN_OPENGL_SOURCE_PATH}/main.cpp
    ${LEARN_OPENGL_SOURCE_PATH}/glad/glad.c
    ${LEARN_OPENGL_SOURCE_PATH}/addStbImage.cpp
    ${LEARN_OPENGL_SOURCE_PATH}/cameraSystem.cpp
    ${LEARN_OPENGL_SOURCE_PATH}/shader.cpp
)

add_executable(learnOpenGL
    ${LEARN_OPENGL_SOURCE}
)

# 根据构建类型设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode")
    # 可以添加特定的 Debug 编译选项，例如 -g
    set_target_properties(learnOpenGL PROPERTIES COMPILE_FLAGS "-g")
else()
    message("Building in Release mode or other mode")
    # 可以添加特定的 Release 编译选项，例如 -O2
    set_target_properties(learnOpenGL PROPERTIES COMPILE_FLAGS "-O2")
endif()

target_link_libraries(
    learnOpenGL
    ${SDK_LIBS}
    # -lpthread -lXrandr -lXi -ldl
)

# 链接系统的 OpenGL 框架
if (APPLE)
    target_link_libraries(learnOpenGL "-framework OpenGL")
endif()
