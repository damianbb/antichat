cmake_minimum_required(VERSION 2.8)
project(antichat_client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pipe -Ofast -lboost_system -pthread")
file(GLOB SOURCE_ED "../external/ed25519_src/*.c" # All files in directory:
)
set(SOURCE_FILES client_tests.cpp
		 c_client.cpp
		 c_user.cpp
		 c_UDPasync.cpp
		 ../external/c_statistics.cpp
		 ../external/c_crypto_ed25519.cpp
		 ${SOURCE_ED}
)
add_executable(antichat_client ${SOURCE_FILES}
)

find_package(Boost COMPONENTS system REQUIRED)

TARGET_LINK_LIBRARIES(antichat_client ${Boost_LIBRARIES})
