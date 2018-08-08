cmake_minimum_required(VERSION 3.1)
project(exception)


set(CMAKE_CXX_STANDARD 11)

set(HEADER_PATH "./include")
set(SOURCE_PATH "./src")
set(PROSTO_PATH "../../include/")

#aux_source_directory(${HEADER_PATH} HEADER_LIST)
aux_source_directory(${SOURCE_PATH} SRC_LIST)
include_directories(${HEADER_PATH} ${PROSTO_PATH})
#source_group("include" FILES ${HEADER_LIST})
file(GLOB HEADER_LIST "${HEADER_PATH}/*.h*")

FIND_PACKAGE( Boost)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -pedantic-errors")

#elseif(MSVC)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /WX")

else()
  message( FATAL_ERROR "unsupported compiler")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DPROSTO_DEBUG")
endif()


add_executable(${PROJECT_NAME} ${HEADER_LIST} ${SRC_LIST})
