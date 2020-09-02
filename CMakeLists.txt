cmake_minimum_required( VERSION 3.16 )
project(UdpReceiver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(UdpReceiver src/UdpReceiver.cpp)
target_link_libraries(UdpReceiver ${CONAN_LIBS})
