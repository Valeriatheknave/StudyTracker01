cmake_minimum_required(VERSION 3.10)
project(StudyTracker VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(study_tracker study_tracker_V01.cpp)

if(MSVC)
  target_compile_options(study_tracker PRIVATE /W4)
else()
  target_compile_options(study_tracker PRIVATE -Wall -Wextra -pedantic)
endif()
