cmake_minimum_required(VERSION 3.20)
project(cubimg VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# --- Packages ---
find_package(CLI11 REQUIRED)

# Try to get the git commit hash
find_package(Git QUIET)
set(GIT_HASH "")
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
endif()

# Configure the version file
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/version.hpp"
)

file(GLOB SOURCE_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_CPP_FILES})


# Strip symbols from the executable for smaller build
if(CMAKE_BUILD_TYPE MATCHES Release)
    find_program(STRIP_TOOL strip)
    if (STRIP_TOOL)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_STRIP} --strip-all $<TARGET_FILE:${PROJECT_NAME}>
            COMMENT "Stripping debug symbols from executable."
            VERBATIM
        )
    else()
        message("Strip not found. Skip stripping symbols from executable.")
    endif()
endif()

# --- Install configuration ---

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES
    ${CMAKE_SOURCE_DIR}/README.md
    ${CMAKE_SOURCE_DIR}/LICENSE
    DESTINATION .
)
file(GLOB OTHER_LICENSES "${CMAKE_SOURCE_DIR}/LICENSE_*")
install(FILES ${OTHER_LICENSES} DESTINATION licenses)

# --- Packing for distribution ---
# > cd build && cpack -C Release

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/dist")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/dist")
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/dist")

set(CPACK_GENERATOR "ZIP")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_PACKAGE_FILE_NAME
    "${PROJECT_NAME}-${PROJECT_VERSION}")
include(CPack)
