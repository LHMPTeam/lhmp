cmake_minimum_required(VERSION 3.2)
project(server)

if(WIN32)
  
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-result -Wno-unused-parameter -std=gnu++11 -Wall -Wextra -Werror")
endif()

set(SOURCE_FILES
    server/main.cpp
)

target_link_libraries(server
	vendor/raknet/Source
)

add_executable(server ${SOURCE_FILES})