# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.20)
project(memex LANGUAGES CXX)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Use C++ 23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Precompiled header
set(MEMEX_PCH "src/pch.hpp")

# Files
file(GLOB_RECURSE MEMEX_SRC CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE MEMEX_HEADERS CONFIGURE_DEPENDS "include/*.hpp")

# Define library target
add_library(memex STATIC ${MEMEX_SRC} ${MEMEX_HEADERS})
add_library(memex::core ALIAS memex)

# Include directories
target_include_directories(memex PUBLIC
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

# PCH
target_precompile_headers(memex PRIVATE ${MEMEX_PCH})

if (MSVC)
  target_compile_options(memex PRIVATE
    /W4              # High warning level
    /WX              # Warnings as errors
    /permissive-     # Enforce standard compliance
    /sdl             # Enable additional security checks
    /Zc:__cplusplus  # Proper __cplusplus macro
  )
else()
  target_compile_options(memex PRIVATE
    -Wall
    -Wextra
    -pedantic-errors # Treat pedantic issues as errors
    -Wconversion     # Warn on implicit type conversions
    -Wsign-conversion
    -Werror          # Treat all warnings as errors
    -Wshadow         # Warn on shadowed variables
    -Wold-style-cast
    -Wduplicated-cond
    -Wnon-virtual-dtor
    -Wunused
    -Wnull-dereference
  )
endif()


# Install rules
install(TARGETS memex EXPORT memexTargets)
install(DIRECTORY include/ DESTINATION include)

#########################
# Uncomment for testing #
#########################

 # add_executable(test_runner src/memex.cpp)
 # target_include_directories(test_runner PRIVATE ${CMAKE_SOURCE_DIR}/include)
 # target_link_libraries(test_runner PRIVATE memex)

