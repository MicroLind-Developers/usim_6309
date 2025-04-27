
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)


include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

add_executable(usim_test
    main.cpp
    term.cpp
)

add_subdirectory(lib)
add_subdirectory(usim_gui)

target_link_libraries(usim_test PRIVATE
    usim
    argparse
)