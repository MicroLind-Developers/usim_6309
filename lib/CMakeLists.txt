
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

project(usim VERSION 1.0)

add_library(${PROJECT_NAME} STATIC 
    src/mc6809.cpp
    src/mc6809in.cpp
    src/mc6309.cpp
    src/mc6309in.cpp
    src/mc6850.cpp
    src/xr88c92.cpp
    src/memory.cpp
    src/usim.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    include/
)

include(TestBigEndian)
test_big_endian(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    target_compile_definitions(${PROJECT_NAME} PUBLIC
        -DBYTE_ORDER_MSB_FIRST
        -DBITFIELDS_LSB_FIRST
    )
endif()
