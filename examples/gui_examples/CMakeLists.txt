cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(gui_examples CXX)

# place binaries and libraries according to GNU standards

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

find_package(Threads)
find_package(cppzmq)

add_executable(
    gui_serives_examples
    gui_serives_examples.cpp
)
target_link_libraries(
    gui_serives_examples
    PRIVATE cppzmq ${CMAKE_THREAD_LIBS_INIT} imgui-gl3
    CatGrayRemoteObjects_a
)

add_executable(
    gui_client_examplesB
    gui_client_examplesB.cpp
)
target_link_libraries(
    gui_client_examplesB
    PRIVATE cppzmq ${CMAKE_THREAD_LIBS_INIT} imgui-gl3
    CatGrayRemoteObjects_a
)

add_executable(
    gui_client_examplesC
    gui_client_examplesC.cpp
)
target_link_libraries(
    gui_client_examplesC
    PRIVATE cppzmq ${CMAKE_THREAD_LIBS_INIT} imgui-gl3
    CatGrayRemoteObjects_a
)

