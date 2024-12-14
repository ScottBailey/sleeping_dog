cmake_minimum_required(VERSION 3.25)
project(sleeping_dog)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



option(SD_BUILD_TEST "Build the tests." Off)
option(SD_BUILD_EXAMPLES "Build the examples." On)



set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Modules" ${CMAKE_MODULE_PATH})

# Add third party dependencies BEFORE adding wall & werror.
add_subdirectory(third_party)

# Warnings as errors.
include(Modules/common.cmake)

add_subdirectory(lib)

if(SD_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
if(BUILD_TEST)
  enable_testing()
  add_subdirectory(test)
endif()
