cmake_minimum_required(VERSION 3.25)

project(
    DoozAI
    VERSION 0.0.1
    LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(dooz "${SRC_DIR}/build.c")

target_include_directories(dooz PRIVATE "${SRC_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/raylib/include")

target_link_directories(dooz PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/raylib/lib")
target_link_libraries(dooz PRIVATE raylib "$<$<NOT:$<BOOL:${MSVC}>>:m>" "$<$<BOOL:${MINGW}>:WinMM>")

set_property(TARGET dooz PROPERTY C_STANDARD 11)
