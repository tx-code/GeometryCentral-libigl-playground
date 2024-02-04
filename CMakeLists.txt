cmake_minimum_required(VERSION 3.28)

project("GeometryCentral libigl playground")
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "In-source builds are not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif ()

### Configure output locations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

### Compile options
set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # Emit a compile_commands.json file for use with clangd or other tools

if (MSVC)
    # using Visual Studio C++
    message("Using Visual Studio compiler flags")
    set(BASE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(BASE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") # parallel build
    SET(DISABLED_WARNINGS "${DISABLED_WARNINGS} /wd\"4267\"")  # ignore conversion to smaller type (fires more aggressively than the gcc version, which is annoying)
    SET(DISABLED_WARNINGS "${DISABLED_WARNINGS} /wd\"4244\"")  # ignore conversion to smaller type (fires more aggressively than the gcc version, which is annoying)
    SET(DISABLED_WARNINGS "${DISABLED_WARNINGS} /wd\"4305\"")  # ignore truncation on initialization
    SET(CMAKE_CXX_FLAGS "${BASE_CXX_FLAGS} ${DISABLED_WARNINGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

    add_definitions(/D "_CRT_SECURE_NO_WARNINGS")
    add_definitions(-DNOMINMAX)
    add_definitions(-D_USE_MATH_DEFINES)
else ()
    # temporarily unrecognized
    message(FATAL_ERROR "Unrecognized compiler [${CMAKE_CXX_COMPILER_ID}]")
endif ()

# Find 3rd party packages
find_package(Eigen3 REQUIRED)
find_package(libigl CONFIG REQUIRED)
find_package(args CONFIG REQUIRED)

add_subdirectory(deps/geometry-central)

add_subdirectory(tutorial)