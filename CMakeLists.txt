cmake_minimum_required(VERSION 3.2)
project(gl)

set(EXECUTABLE_OUTPUT_PATH bin)
set(TARGET main)
aux_source_directory(src SRCS)
aux_source_directory(src/object SRCS)
aux_source_directory(src/scene SRCS)
aux_source_directory(src/shader SRCS)
aux_source_directory(src/builder SRCS)
add_executable(${TARGET} ${SRCS})

file (GLOB ASSETS_FULL assets/*)
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    foreach(item_full IN LISTS ASSETS_FULL)
        file (RELATIVE_PATH item ${CMAKE_CURRENT_SOURCE_DIR} ${item_full})
        set(ASSETS ${ASSETS} ${item})
        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${item}"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${item}" "${CMAKE_CURRENT_BINARY_DIR}/${item}"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
        )
    endforeach()
endif()
add_custom_target(assets ALL DEPENDS ${ASSETS})

include_directories(include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/src/,,$(abspath $<))\"'")

find_package(OpenGL REQUIRED)
target_link_libraries(${TARGET} ${OPENGL_LIBRARIES})

find_library(GLFW_LIBRARIES glfw)
target_link_libraries(${TARGET} ${GLFW_LIBRARIES})

find_library(GLEW_LIBRARIES glew)
target_link_libraries(${TARGET} ${GLEW_LIBRARIES})

find_library(PNG_LIBRARIES png)
target_link_libraries(${TARGET} ${PNG_LIBRARIES})

find_library(MMD_TOOLS mmd-tools PATHS lib)
target_link_libraries(${TARGET} ${MMD_TOOLS})

find_library(MMD_PHY mmd-physics PATHS lib)
target_link_libraries(${TARGET} ${MMD_PHY})
