# #########################################
# Initial config
# #########################################
set(LIBRARY_TARGET_NAME SupergoonEngine)
set(EXECUTABLE_TARGET_NAME SupergoonEngineTests)
cmake_minimum_required(VERSION 3.16)
message(STATUS "Starting ${LIBRARY_TARGET_NAME} project initialization")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # This exports a file that clangd can use so that we know what compile/include flags so that we can use clangd language server properly with our cmake attributes.
include(FindPkgConfig)
project(${LIBRARY_TARGET_NAME} VERSION 0.1.0 LANGUAGES C)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# #########################################
# File globbing
# #########################################
message(STATUS "Getting all files for cmake..")
file(GLOB_RECURSE FILES src/GoonEngine/*.c*)
file(GLOB_RECURSE TESTFILES src/tests/*.c*)

# #########################################
# Platform specific Project setup
# #########################################
message(STATUS "Changing the platform specific settings")
if(EMSCRIPTEN)
    message(STATUS "Configuring Emscripten")
elseif(APPLE)
    if(NOT IOS) # MACOS #
        message(STATUS "Configuring Macos")
        set(MACOS ON)
        if(CMAKE_GENERATOR STREQUAL "Xcode")
            message(STATUS "Setting Build to Universal Binary")
            set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" FORCE)
        endif(CMAKE_GENERATOR STREQUAL "Xcode")
    else()
    # IOS
    endif(NOT IOS)
elseif(WIN32)
    message(STATUS "Configuring Windows")
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /c/cmake)
elseif(UNIX AND NOT APPLE)
    message(STATUS "Configuring Linux")
    set(LINUX ON)
endif(EMSCRIPTEN)

# #########################################
# Initialize
# #########################################
message(STATUS "Initializing the project")
add_library(${LIBRARY_TARGET_NAME}
    SHARED
    ${FILES}
)
set_property(TARGET ${LIBRARY_TARGET_NAME} PROPERTY C_STANDARD 99)
find_package(PkgConfig REQUIRED)
include(CTest)

# #########################################
# Options
# #########################################
option(CMAKE_DEBUG_VARIABLES "Runs a debug on all variables for troubleshooting" OFF)
option(GOON_BUILD_PCH "Uses a PCH file to try and speed up compilation" ON)
option(GOON_DEBUG_LUA "Includes socket so that we can debug lua remotely" OFF)
option(GOON_MULTITHREADED_SOUND "Adds multithreaded support for sound updates, experimental and has a memory leak somehow" OFF)
option(GOON_BENCHMARKING "Enables benchmarking functions, adds overhead" OFF)
option(GOON_BUILD_TESTING "Should we enable testing with ctest" OFF)
set(BUILD_TESTING GOON_BUILD_TESTING) # Set the cmake variable build_testing to our option
# option(GOON_FULL_MACOS_BUILD "Full builds of all libraries, used for runners mostly, and passed in to override." OFF)

# #########################################
# Precompiled headers
# #########################################
if(GOON_BUILD_PCH)
    message(STATUS "Setting up PCH, mac needs objc")
    target_precompile_headers(${LIBRARY_TARGET_NAME} PRIVATE
        "$<$<COMPILE_LANGUAGE:C>:${CMAKE_CURRENT_SOURCE_DIR}/include/GoonEngine/gnpch.h>")
endif(GOON_BUILD_PCH)

# #########################################
# Build Type specific setup
# #########################################
# If we are debug, we should use the local path, otherwise use the location of embuilder from the docker image
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${LIBRARY_TARGET_NAME} PRIVATE -DGN_DEBUG_BUILD)
else()
    target_compile_definitions(${LIBRARY_TARGET_NAME} PRIVATE -DGN_RELEASE_BUILD)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

# #########################################
# Packages
# #########################################
# 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2_image
    NAMES SDL2_Image libSDL2_Image SDL2_image libSDL2_image libSDL2_image
    CONFIG COMPONENTS SDL2_image
    HINTS /usr/local/lib)
find_package(SDL2_ttf
    NAMES SDL2_ttf libSDL2_ttf SDL2_Ttf libSDL2_Ttf libSDL2_Ttf
    CONFIG COMPONENTS SDL2_ttf
    HINTS /usr/local/lib)
# 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

# Build my Libraries and include directories regardless
add_subdirectory(./external/goonPhysics)
target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC external/goonPhysics/include/)
add_subdirectory(./external/sg_sound)
target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC external/sg_sound/src/)

# #########################################
# Platform Specific Build settings
# #########################################


# #########################################
# Compile Definitions
# #########################################

# Platform-specific compile definitions
target_compile_definitions(${LIBRARY_TARGET_NAME} PRIVATE
    $<$<BOOL:${APPLE}>: -DGN_PLATFORM_MACOS>
    $<$<BOOL:${WIN32}>: -DGN_PLATFORM_WINDOWS>
    $<$<BOOL:${EMSCRIPTEN}>: -DGN_PLATFORM_EMSCRIPTEN>
    $<$<BOOL:${LINUX}>: -DGN_PLATFORM_LINUX>
)
# Other compile definitions
target_compile_definitions(${LIBRARY_TARGET_NAME} PRIVATE
    $<$<BOOL:${GOON_MULTITHREADED_SOUND}>: -DGN_MULTITHREADED>
    $<$<BOOL:${GOON_DEBUG_LUA}>: -DGN_DEBUG_LUA>
    $<$<BOOL:${GOON_BENCHMARKING}>: -DGN_BENCHMARKING_ENABLED>
)

# #########################################
# Debug
# #########################################
if(CMAKE_DEBUG_VARIABLES)
    message(STATUS "Exporting debug")
    get_cmake_property(_variableNames VARIABLES)
    list(SORT _variableNames)

    foreach(_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endif(CMAKE_DEBUG_VARIABLES)

# #########################################
# Compile Options
# #########################################
set(FULL_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic
    -Wdeprecated
    -Wno-newline-eof
    -Wno-strict-prototypes
    -pthread
)

if(WIN32)
    set(FULL_COMPILE_OPTIONS /std:c99)
elseif(EMSCRIPTEN)
    # target_compile_options(${EXECUTABLE_TARGET_NAME} PRIVATE -sUSE_SDL_TTF=2)
endif(WIN32)


target_compile_options(${LIBRARY_TARGET_NAME} PRIVATE ${FULL_COMPILE_OPTIONS})

# #########################################
# Linking Options
# #########################################

set(LIBRARIES_TO_LINK
    SDL2::SDL2main
    SDL2::SDL2-static
    SDL2_image::SDL2_image-static
    goonPhysics
    supergoonSound
    SupergoonUI
)

foreach(LIBRARY ${LIBRARIES_TO_LINK})
    # Check if the target exists, if we are building it then we should link to it.
    if(TARGET ${LIBRARY})
        target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE ${LIBRARY})
    endif()
endforeach()

target_link_libraries(${LIBRARY_TARGET_NAME}
    PUBLIC
    PRIVATE
    ${LUA_LIBRARIES}
    ${GOONSOUND_LIBRARIES}
    ${GOONPHYSICS_LIBRARIES}
    ${LIB_SOCKET}
)
target_link_directories(${LIBRARY_TARGET_NAME}
    PRIVATE
    /usr/local/lib
)

target_include_directories(${LIBRARY_TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    /usr/local/include
)

# Tests
add_executable(${EXECUTABLE_TARGET_NAME}
    ${TESTFILES})
target_link_libraries(${EXECUTABLE_TARGET_NAME}
    PUBLIC
    PRIVATE
    SupergoonEngine
)
target_include_directories(${EXECUTABLE_TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)