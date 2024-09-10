cmake_minimum_required(VERSION 3.9)

project(CatGrayRemoteObjects VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

file(GLOB SRC_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)


add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE cppzmq
)

target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(${PROJECT_NAME}_a STATIC ${SRC_LIST})
target_link_libraries(
    ${PROJECT_NAME}_a
    PRIVATE cppzmq
)

target_include_directories(${PROJECT_NAME}_a INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

# target_link_libraries(${PROJECT_NAME} PRIVATE libzmq)
# target_link_libraries(${PROJECT_NAME}_a PRIVATE libzmq-static)