cmake_minimum_required(VERSION 3.22)
# Set vcpkg toolchain
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
else()
    message(FATAL_ERROR "Please set the VCPKG_ROOT environment variable to your vcpkg root directory.")
endif()

# Set the project name
project(EntWars)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_subdirectory(reflect-cpp)
set(REFLECTCPP_YAML ON)

# Add the executable
add_executable(EntWars ${SOURCES})
# Add precompiled header
target_precompile_headers(EntWars PRIVATE src/pch.h)

# Link libraries
find_package(SFML CONFIG REQUIRED COMPONENTS system window graphics network audio)
find_package(entt CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(EASTL CONFIG REQUIRED)

target_link_libraries(EntWars 
	PRIVATE 
		sfml-system 
		sfml-window 
		sfml-graphics 
		sfml-network 
		sfml-audio
	)
target_link_libraries(EntWars
 PRIVATE 
	EnTT::EnTT
 )
target_link_libraries(EntWars
  PUBLIC
    ImGui-SFML::ImGui-SFML
)
target_link_libraries(EntWars
  PUBLIC
    EASTL
)

if (CMAKE_VERSION VERSION_GREATER 3.7.8)
    if (MSVC_IDE)
        option(VS_ADD_NATIVE_VISUALIZERS "Configure project to use Visual Studio native visualizers" TRUE)
    else()
        set(VS_ADD_NATIVE_VISUALIZERS FALSE CACHE INTERNAL "Native visualizers are Visual Studio extension" FORCE)
    endif()

    # add natvis file to the library so it will automatically be loaded into Visual Studio
    if(VS_ADD_NATIVE_VISUALIZERS)
        target_sources(EASTL INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/EASTL.natvis
        )
    endif()
endif()

target_link_libraries(EntWars PRIVATE reflectcpp)

# Set the output directory for the executable
set_target_properties(EntWars PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable cross-platform building
if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)
endif()