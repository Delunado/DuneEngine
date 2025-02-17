cmake_minimum_required(VERSION 3.20)
project(DuneEngine)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Detect architecture
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_DIR "x64")
else ()
    set(ARCH_DIR "x86")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${ARCH_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${ARCH_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${ARCH_DIR}/${CMAKE_BUILD_TYPE})

# Define dependency paths
include_directories(include
        external/glm
        external/SDL2/include
        external/SDL2_mixer/include)

link_directories(link
        "external/SDL2/lib/${ARCH_DIR}"
        "external/SDL2_mixer/lib/${ARCH_DIR}")

# Add source files
file(GLOB_RECURSE ENGINE_SOURCES src/*.cpp)
add_executable(DuneEngine ${ENGINE_SOURCES})

# Link libraries
target_link_libraries(DuneEngine
        SDL2main
        SDL2
        SDL2_mixer)