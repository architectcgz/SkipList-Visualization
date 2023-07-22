cmake_minimum_required(VERSION 3.25)
project(SkipList_SFML)

set(CMAKE_CXX_STANDARD 17)

add_executable(SkipList_SFML main.cpp SkipList.cpp SkipList.h)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:\\SFML\\lib\\cmake\\SFML)
find_package(SFML COMPONENTS audio network graphics window system REQUIRED)
include_directories(C:\\SFML\\include)
target_link_libraries(SkipList_SFML sfml-audio sfml-network sfml-graphics sfml-window sfml-system -static )
