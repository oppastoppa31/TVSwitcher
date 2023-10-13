project(TVSwitcher)
cmake_minimum_required(VERSION 3.22)
file(GLOB SRC_FILES *.c *.cpp) # make this output a shared library (with .so
                               # output)
add_library(wiringPi SHARED ${SRC_FILES})
target_include_directories(wiringPi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(wiringPi ${CMAKE_THREAD_LIBS_INIT})
