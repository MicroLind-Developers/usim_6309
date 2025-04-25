cmake_minimum_required(VERSION 3.5)
project(usim_gui VERSION 1.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find wxWidgets
find_package(wxWidgets COMPONENTS core base stc REQUIRED)
include(${wxWidgets_USE_FILE})

# Include directories for project headers and third-party libs
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${wxWidgets_INCLUDE_DIRS}
    # Add path to argparse.hpp if installed in a custom location
    # ${CMAKE_CURRENT_SOURCE_DIR}/third_party/argparse/include
)

# Source files
set(SOURCES
    src/main.cpp
    src/CPUModel.cpp
    src/MainView.cpp
    src/MainController.cpp
)

# Define executable
add_executable(usim_gui ${SOURCES})

# Add library search path
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build)

target_link_libraries(usim_gui
    ${wxWidgets_LIBRARIES}
    pthread
    usim
)
