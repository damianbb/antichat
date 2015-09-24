cmake_minimum_required(VERSION 3.3)
project(antichat_client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Ofast -pipe -lboost_system")

set(SOURCE_FILES main.cpp)

add_executable(antichat_client ${SOURCE_FILES}
        c_statistics.hpp c_statistics.cpp
        c_UDPasync.hpp c_UDPasync.cpp
        c_locked_queue.hpp
        log.hpp
)

find_package(Boost COMPONENTS system REQUIRED)

TARGET_LINK_LIBRARIES(antichat_client ${Boost_LIBRARIES})