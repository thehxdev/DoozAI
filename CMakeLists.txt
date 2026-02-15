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

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    set(CMAKE_TLS_VERIFY FALSE)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
    endif()
endif()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(dooz "${SRC_DIR}/build.c")
target_include_directories(dooz PRIVATE "${SRC_DIR}")

if (WIN32)
    target_link_libraries(dooz PRIVATE raylib "$<$<NOT:$<BOOL:${MSVC}>>:m>" "$<$<BOOL:${MINGW}>:WinMM>")
    target_compile_options(dooz PRIVATE $<$<BOOL:${MINGW}>:-msse4.1>)
else()
    target_link_libraries(dooz PRIVATE raylib m)
    target_compile_options(dooz PRIVATE -msse4.1)
endif()

set_property(TARGET dooz PROPERTY C_STANDARD 11)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets/board.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
