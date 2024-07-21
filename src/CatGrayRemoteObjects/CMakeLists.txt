cmake_minimum_required(VERSION 3.9)

project(CatGrayRemoteObjects VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CPPZMQ_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/private/zmq.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/private/zmq_addon.hpp
)

add_library(${PROJECT_NAME} SHARED ${CPPZMQ_HEADERS})
target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(${PROJECT_NAME}_a STATIC ${CPPZMQ_HEADERS})
target_include_directories(${PROJECT_NAME}_a INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

# target_link_libraries(${PROJECT_NAME} PRIVATE libzmq)
# target_link_libraries(${PROJECT_NAME}_a PRIVATE libzmq-static)