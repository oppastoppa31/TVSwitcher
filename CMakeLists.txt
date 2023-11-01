project(TVSwitcher LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)

# CHANGE THIS FOR YOUR BUILD
set(RPI_IP raspberrypi.local)
set(RPI_USER cyber)
set(RPI_DIR ~)
# END

configure_file(install.sh.in install.sh @ONLY)

file(GLOB SRC_FILES *.c *.cpp)

add_executable(TVSwitcher ${SRC_FILES})
target_link_libraries(TVSwitcher pigpio rt)
add_definitions(-pthread)
target_include_directories(TVSwitcher PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
                                             "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
target_link_directories(TVSwitcher PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
install(
  CODE "execute_process(COMMAND ${CMAKE_CURRENT_BINARY_DIR}/install.sh ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_NAME})"
)
add_definitions(-Wall -Wextra -Wpedantic -Werror -Wno-comment)
