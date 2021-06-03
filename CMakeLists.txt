cmake_minimum_required(VERSION 3.17)
project(cpp_study CXX)

set(CMAKE_CXX_STANDARD 17)
#set( CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -stdlib=libc++")
# Set up some extra Conan dependencies based on our needs before loading Conan
set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "boost:header_only=True")

include(conan.cmake)
run_conan()

find_package(Boost REQUIRED)

add_executable(cpp_study main.cpp)
target_link_libraries(
        cpp_study
        CONAN_PKG::websocketpp
        CONAN_PKG::fmt
        CONAN_PKG::spdlog)