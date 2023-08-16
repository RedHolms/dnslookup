cmake_minimum_required(VERSION 3.8)

project("dnslookup" C)

add_executable(dnslp "main.c")

if(WIN32)
  target_link_libraries(dnslp PRIVATE ws2_32)
endif()