cmake_minimum_required(VERSION 3.6)

project(rsa LANGUAGES CXX)

find_package(Boost REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

add_executable(rsa-keygen src/rsa_keygen.cpp)
target_include_directories(rsa-keygen PRIVATE include ${Boost_INCLUDE_DIRS})
target_link_libraries(rsa-keygen PRIVATE ${Boost_LIBRARIES})

add_executable(rsa-encode src/rsa_encode.cpp)
target_include_directories(rsa-encode PRIVATE include ${Boost_INCLUDE_DIRS})
target_link_libraries(rsa-encode PRIVATE ${Boost_LIBRARIES})

add_executable(rsa-decode src/rsa_decode.cpp)
target_include_directories(rsa-decode PRIVATE include ${Boost_INCLUDE_DIRS})
target_link_libraries(rsa-decode PRIVATE ${Boost_LIBRARIES})

add_executable(tests tests/main.cpp tests/rsa.cpp)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE Catch2::Catch2)
