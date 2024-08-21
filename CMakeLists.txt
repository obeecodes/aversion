cmake_minimum_required(VERSION 3.24.4)

# Set project name
project(Aversion)

# Set the C++ compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "Suppresses developer warnings.")
set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX14.5.sdk)

# Find SDL2 and related libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Set library search paths if needed (though often not necessary with find_package)
link_directories(
  /opt/homebrew/Cellar/sdl2/2.30.5/lib
  /opt/homebrew/Cellar/sdl2_mixer/2.8.0/lib
  /opt/homebrew/Cellar/sdl2_image/2.8.2_1/lib
  /opt/homebrew/Cellar/sdl2_ttf/2.22.0/lib
)

# Include directories for SDL2 and related libraries
include_directories(
  ${SDL2_INCLUDE_DIRS}
  ${SDL2_MIXER_INCLUDE_DIRS}
  ${SDL2_IMAGE_INCLUDE_DIRS}
  ${SDL2_TTF_INCLUDE_DIRS}
)

# Add Lua subdirectory
add_subdirectory(external/lua)

# Gather source files
file(GLOB SOURCES src/*.cpp src/*/*.cpp src/*/*/*.cpp include/*.h include/*/*.h include/*/*/*.h)

# Set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Define the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Set C++ standard
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Link SDL2 and related libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
  ${SDL2_LIBRARIES}
  ${SDL2_MIXER_LIBRARIES}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
)

# Link Lua
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC lua)

# Link SDL2_mixer using its imported target
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf)
# Add linker flag -Wl,-ld_classic
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -Wl,-ld_classic)
