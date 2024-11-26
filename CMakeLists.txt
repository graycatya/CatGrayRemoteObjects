cmake_minimum_required(VERSION 3.9)

project(CatGrayRemoteObjects-Projects LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#if (UNIX)
#add_compile_options(-Wall -fPIC -Wno-stringop-overflow)
#endif(UNIX)

message(STATUS "------------------------------------------------------------" )
message(STATUS "[${PROJECT_NAME}] Configuration summary."                     )
message(STATUS "------------------------------------------------------------ ")
message(STATUS " System configuration:"                                       )
message(STATUS " .. Processor type .............. = ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS " .. CMake executable ............ = ${CMAKE_COMMAND}"         )
message(STATUS " .. CMake version ............... = ${CMAKE_VERSION}"         )
message(STATUS " .. System name ................. = ${CMAKE_SYSTEM}"          )
message(STATUS " .. C++ compiler ................ = ${CMAKE_CXX_COMPILER}"    )
message(STATUS " .. C compiler .................. = ${CMAKE_C_COMPILER}"      )
message(STATUS " .. size(void*) ................. = ${CMAKE_SIZEOF_VOID_P}"   )
message(STATUS " .. cmake current source ................. = ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS " .. cmake system name ................. = ${CMAKE_SYSTEM_NAME}")
message(STATUS "------------------------------------------------------------ ")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# build option
option(CATGRAYREMOTEOBJECTSPROJECTS_EXAMPLES "Build examples" OFF)
option(IMGUI_TEST "Build imgui test" OFF)
set(FOLDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui-gl3/imgui")

if(EXISTS ${FOLDER_PATH})
    file(GLOB FILES_IN_FOLDER "${FOLDER_PATH}/*")
    if(NOT FILES_IN_FOLDER)
        set(IMGUI_TEST OFF)
    else()
    endif()
else()
    set(IMGUI_TEST OFF)
endif()

set(3RDPARTY
    libzmq
    cppzmq
    imgui-gl3
)


set(MODULES
    CatGrayRemoteObjects
)

set(EXAMPLES
    cppzmq_examples
    gui_examples
)

foreach(3rdparty_module ${3RDPARTY})
    if(${3rdparty_module} STREQUAL "libzmq")
        # set(ENABLE_PRECOMPILED OFF
        #     CACHE BOOL "ENABLE_PRECOMPILED OFF" FORCE)
        set(BUILD_TESTS OFF
            CACHE BOOL "BUILD_TESTS OFF" FORCE)
        set(ZMQ_BUILD_TESTS OFF
            CACHE BOOL "BUILD_TESTS OFF" FORCE)
        set(BUILD_STATIC OFF)
        set(WITH_PERF_TOOL OFF
            CACHE BOOL "WITH_PERF_TOOL OFF" FORCE)
    endif()

    if(${3rdparty_module} STREQUAL "cppzmq")
        set(CPPZMQ_BUILD_TESTS OFF
            CACHE BOOL "CPPZMQ_BUILD_TESTS OFF" FORCE)
    endif()

    if(NOT ${3rdparty_module} STREQUAL "imgui-gl3")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${3rdparty_module})
    else()
        if(IMGUI_TEST)
            add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${3rdparty_module})
        endif()
    endif()

endforeach(3rdparty_module ${3RDPARTY})

foreach(module ${MODULES})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/${module})
endforeach(module ${MODULES})
if(CATGRAYREMOTEOBJECTSPROJECTS_EXAMPLES)
foreach(example ${EXAMPLES})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/${example})
endforeach(example ${EXAMPLES})
endif()
