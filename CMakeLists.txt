cmake_minimum_required(VERSION 3.2)
project(gl)

find_package(PkgConfig REQUIRED)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

set(EXECUTABLE_OUTPUT_PATH bin)
set(TARGET main)
aux_source_directory(src SRCS)
aux_source_directory(src/core SRCS)
aux_source_directory(src/object SRCS)
aux_source_directory(src/component SRCS)
aux_source_directory(src/script SRCS)
aux_source_directory(src/shader SRCS)
aux_source_directory(src/builder SRCS)
add_executable(${TARGET} ${SRCS})

file (GLOB_RECURSE ASSETS_FULL assets/*)
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    foreach(item_full IN LISTS ASSETS_FULL)
        file (RELATIVE_PATH item ${CMAKE_CURRENT_SOURCE_DIR} ${item_full})
        set(ASSETS ${ASSETS} ${item})
        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${item}"
            COMMAND ${CMAKE_COMMAND} -E copy '${CMAKE_CURRENT_SOURCE_DIR}/${item}' '${CMAKE_CURRENT_BINARY_DIR}/${item}'
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
        )
    endforeach()
endif()
add_custom_target(assets ALL DEPENDS ${ASSETS})

include_directories(include)
include_directories(../mmd-tools/include)
include_directories(../mmd-physics/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PNG REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${PNG_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})

target_link_libraries(${TARGET} ${OPENGL_LIBRARIES})
target_link_libraries(${TARGET} ${GLFW_LIBRARIES})
target_link_libraries(${TARGET} ${GLEW_LIBRARIES})
target_link_libraries(${TARGET} ${PNG_LIBRARIES})

find_library(MMD_TOOLS mmd-tools PATHS ../mmd-tools/build/lib)
target_link_libraries(${TARGET} ${MMD_TOOLS})

find_library(MMD_PHY mmd-physics PATHS ../mmd-physics/build/lib)
target_link_libraries(${TARGET} ${MMD_PHY})

if (WIN32)
add_custom_command(TARGET ${TARGET} POST_BUILD
             COMMAND ${CMAKE_COMMAND} -E copy_if_different
             ${MMD_TOOLS} $<TARGET_FILE_DIR:${TARGET}>)
 add_custom_command(TARGET ${TARGET} POST_BUILD
             COMMAND ${CMAKE_COMMAND} -E copy_if_different
             ${MMD_PHY} $<TARGET_FILE_DIR:${TARGET}>)
endif(WIN32)
