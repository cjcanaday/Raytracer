cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard
project(RayTracer CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use RelWithDebInfo for extra performance. This probably only matters on Windows since we directly set -g and -O3 on GNU/Clang
set(CMAKE_BUILD_TYPE "RelWithDebInfo")

# Find Homebrew path on MacOS
if(APPLE)
    execute_process(COMMAND brew --prefix OUTPUT_VARIABLE BREWPATH OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

# Setup compiler flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_definitions(_USE_MATH_DEFINES)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244") # Disable warning C4244 (conversion from 'double' to 'float')
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -Wno-deprecated-register -Wno-deprecated-declarations -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -no-pie")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -Wno-deprecated-register -Wno-deprecated-declarations -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED")
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set source directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_SOURCE_DIR}/include)

# Gather source files
file(GLOB_RECURSE SRC_FILES 
    "main.cpp"
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/scenes/*.inl"  # Ensure inlined files are tracked
)

# Include directories
include_directories(${INC_DIR})

# Define executable
add_executable(RayTracer ${SRC_FILES})

# Link required libraries
if(WIN32)
    target_link_libraries(RayTracer
        "${CMAKE_SOURCE_DIR}/lib/windows/glew32.lib"
        "${CMAKE_SOURCE_DIR}/lib/windows/freeglut.lib"
        "${CMAKE_SOURCE_DIR}/lib/windows/FreeImage.lib"
    )
    # Ensure .dll is with .exe
    file(COPY "${CMAKE_SOURCE_DIR}/lib/windows/glew32.dll"    DESTINATION "${CMAKE_BINARY_DIR}/bin")
    file(COPY "${CMAKE_SOURCE_DIR}/lib/windows/freeglut.dll"  DESTINATION "${CMAKE_BINARY_DIR}/bin")
    file(COPY "${CMAKE_SOURCE_DIR}/lib/windows/FreeImage.dll" DESTINATION "${CMAKE_BINARY_DIR}/bin")
elseif(APPLE)
    include_directories(RayTracer
        ${BREWPATH}/include
    )
    target_link_libraries(RayTracer 
        "-framework OpenGL"
        "-framework GLUT"
        ${BREWPATH}/lib/libfreeimage.dylib
    )
else() # Linux
    target_link_libraries(RayTracer
        "${CMAKE_SOURCE_DIR}/lib/linux/libGLEW.a"
        "glut"
        "GLU"
        "GL"
        "X11"
        "${CMAKE_SOURCE_DIR}/lib/linux/libfreeimage.a"
    )
endif()
