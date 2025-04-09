cmake_minimum_required(VERSION 3.10)

# Project name and language
project(GLESWin32Project LANGUAGES C CXX)


# Add source files
set(SOURCES src/glewesTest.cpp src/glewes.c)

# Add Vulkan include directory (assuming "vulkan/" is at the root)
include_directories(${CMAKE_SOURCE_DIR})

# Create the executable
add_executable(GLESTestApp ${SOURCES})

# Cross-platform support (use Win32 subsystem if on Windows)
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    target_link_libraries(GLESTestApp user32)  # Example: Link user32.lib for a Win32 app
endif()
