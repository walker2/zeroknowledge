cmake_minimum_required(VERSION 3.5)
project(zeroknowledge)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Verifier.cpp Verifier.h Prover.cpp Prover.h ITransportClient.h Utils.h)
add_executable(zeroknowledge ${SOURCE_FILES})
target_link_libraries(zeroknowledge ntl)