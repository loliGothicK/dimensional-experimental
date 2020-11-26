cmake_minimum_required(VERSION 3.16) # Because CLion supports >= 3.16.x
enable_language(CXX)
set(CMAKE_CXX_STANDARD 20) # C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++17
find_package(Threads REQUIRED)
#find_package(Boost 1.72.0 REQUIRED)
## Set our project name
project(dimensional-experimental)

include_directories(
  PUBLIC ${Boost_INCLUDE_DIRS}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(test ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)
add_executable(intro ${CMAKE_CURRENT_SOURCE_DIR}/src/intro.cpp)
