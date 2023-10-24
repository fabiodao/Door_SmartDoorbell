cmake_minimum_required(VERSION 3.16.3)
project( doorbell LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
#set(BUILD_SHARED_LIBS ON)

# packages
find_package(OpenCV REQUIRED)

find_package(Threads REQUIRED)

find_package(Python REQUIRED)

# Compilers
set(CMAKE_CXX_COMPILER "/home/vitor/buildroots/Fabio/buildroot-2021.08/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-g++")
set(CMAKE_CC_COMPILER "/home/vitor/buildroots/Fabio/buildroot-2021.08/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-gcc")


# bin files to bin dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")


set (CMAKE_CXX_FLAGS "-g -lpthread -lpython3.9 -lrt")


include_directories( include /home/vitor/buildroots/Fabio/buildroot-2021.08/output/target/usr/lib)

include_directories( include ${OpenCV_INCLUDE_DIRS} )
include_directories( include ${CMAKE_CURRENT_SOURCE_DIR}/inc/ )
include_directories( include ${CMAKE_CURRENT_SOURCE_DIR}/../inc )

# All Source files
file(GLOB all_SRCS
   "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
   "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp"
)

# Main
add_executable( mainlocal.elf  mainlocal.cpp ${all_SRCS} ) 
target_link_libraries( mainlocal.elf ${OpenCV_LIBS} ${CMAKE_CXX_FLAGS} )
