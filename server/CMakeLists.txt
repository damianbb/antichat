cmake_minimum_required(VERSION 3.0)
project(antichat_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Ofast -std=c++11 -pthread -lboost_system")

set(SOURCE_FILES main.cpp c_server.cpp c_server.hpp)
add_executable(antichat_server ${SOURCE_FILES}
        c_network_api.cpp c_network_api.hpp
        c_user.cpp c_user.hpp
        c_room.cpp c_room.hpp

        #external libs
        ../external/c_statistics.cpp ../external/c_statistics.hpp
        ../external/log.hpp
        ../external/c_locked_queue.hpp
        ../external/ed25519_src/add_scalar.c ../external/ed25519_src/fe.c ../external/ed25519_src/ge.c #ed25519 libs #1
        ../external/ed25519_src/key_exchange.c #ed25519 libs #1
        ../external/ed25519_src/sc ../external/ed25519_src/seed ../external/ed25519_src/sign.c #ed25519 libs #2
        ../external/ed25519_src/verify.c ../external/ed25519_src/keypair.c ../external/ed25519_src/sha512 #ed25519 libs #3
        ../external/c_crypto_ed25519.cpp ../external/c_crypto_ed25519.hpp
)

find_package(Boost COMPONENTS system REQUIRED)

TARGET_LINK_LIBRARIES(antichat_server ${Boost_LIBRARIES})