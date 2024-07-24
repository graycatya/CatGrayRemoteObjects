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
)

add_executable(
    gui_client_examples
    gui_client_examples.cpp
)
target_link_libraries(
    gui_client_examples
    PRIVATE cppzmq ${CMAKE_THREAD_LIBS_INIT} imgui-gl3
)

