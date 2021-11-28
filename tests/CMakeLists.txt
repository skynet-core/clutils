cmake_minimum_required(VERSION 3.20)

message(STATUS ${CMAKE_TOOLCHAIN_FILE})
project(clutils VERSION 0.1.0)

include_directories(../include ../lib)

file(GLOB ${PROJECT_NAME}_TEST_SOURCES *.cpp)

add_executable(${PROJECT_NAME}_test "${${PROJECT_NAME}_TEST_SOURCES}")

find_package(fmt CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)
find_package(OpenCL REQUIRED)
target_link_libraries(
                      ${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}::${PROJECT_NAME} 
                       OpenCL::OpenCL 
                       Catch2::Catch2
                       fmt::fmt-header-only)
