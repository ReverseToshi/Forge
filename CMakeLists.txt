
cmake_minimum_required(VERSION 3.20)

project(Forge
    VERSION 0.1.0
    DESCRIPTION "A high-performance HTTP/1.1 server built in modern C++"
    LANGUAGES CXX
)

find_package(Threads REQUIRED)

add_executable(Forge
    src/main.cpp
    src/Forge.cpp
    src/socket.cpp
)

target_compile_features(Forge PRIVATE cxx_std_20)

set_target_properties(Forge PROPERTIES
    CXX_EXTENSIONS OFF
)

target_include_directories(Forge
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(Forge
    PRIVATE Threads::Threads
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Forge PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
)
endif()

# TESTING

include(CTest)
find_package(GTest REQUIRED)

add_executable(SocketTests
    tests/socket_tests.cpp
    src/socket.cpp
)

target_compile_features(SocketTests PRIVATE cxx_std_20)

target_include_directories(SocketTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SocketTests PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(SocketTests)