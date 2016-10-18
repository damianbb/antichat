project(antichat)
cmake_minimum_required(VERSION 3.2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# add_subdirectory(client) # broken?
add_subdirectory(client2)
add_subdirectory(server)
