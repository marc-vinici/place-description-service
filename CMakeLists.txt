cmake_minimum_required(VERSION 3.14)
project(PlaceDescriptionService)

set(CMAKE_CXX_STANDARD 14)

enable_testing()

set(sources
    Main.cpp
    PlaceDescriptionService.cpp
    PlaceDescriptionServiceTest.cpp
    AddressExtractor.cpp
    CurlHttp.cpp
    CurlHttpTest.cpp)

add_executable(PlaceDescriptionServiceTest ${sources})

find_package(GTest CONFIG REQUIRED)

target_link_libraries(
    PlaceDescriptionServiceTest
    GTest::gmock_main
    GTest::gtest
    jsoncpp
    curl
)

include(GoogleTest)
gtest_discover_tests(PlaceDescriptionServiceTest)