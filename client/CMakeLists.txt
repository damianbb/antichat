cmake_minimum_required(VERSION 2.8)
project(coin_sending)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pipe -Ofast -lboost_system -pthread")

set(SOURCE_FILES client_tests.cpp
                 c_UDPasync.cpp
)
add_executable(coin_sending ${SOURCE_FILES}
)

find_package(Boost COMPONENTS system REQUIRED)

TARGET_LINK_LIBRARIES(coin_sending ${Boost_LIBRARIES})
