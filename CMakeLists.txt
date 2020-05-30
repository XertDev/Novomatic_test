cmake_minimum_required(VERSION 3.15)
project(novomatic)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

enable_testing()
find_package(GTest)

add_library(morph MorphPicker.cpp MorphPicker.hpp)

add_executable(novoTest MorphPickerTest.cpp CircularBufferTest.cpp CircularBuffer.hpp)
target_link_libraries(novoTest GTest::GTest gmock gtest_main)
target_link_libraries(novoTest morph)

add_test( test novoTest )