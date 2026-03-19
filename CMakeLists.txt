cmake_minimum_required(VERSION 3.25)

project(window LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

find_package(glfw3 REQUIRED)

set(SRC src)
set(INCLUDE include)

add_executable(${PROJECT_NAME}
    ${SRC}/glad.c
    ${SRC}/main.cpp
    ${SRC}/Callbacks.cpp
    ${SRC}/Colour.cpp
    ${SRC}/Window.cpp
    ${SRC}/WindowManager.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw stdc++exp)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic)
    set_source_files_properties(${SRC}/glad.c PROPERTIES COMPILE_OPTIONS -Wno-pedantic)
endif()

