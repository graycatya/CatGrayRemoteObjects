cmake_minimum_required(VERSION 3.9)

project(CatGrayRemoteObjects-Projects LANGUAGES CXX)

if (UNIX)
add_compile_options(-Wall -fPIC -Wno-stringop-overflow)
endif(UNIX)

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



set(3RDPARTY
    libzmq
    cppzmq
)


set(MODULES
#    CatGrayRemoteObjects
)

set(EXAMPLES
    cppzmq_examples
)

foreach(3rdparty_module ${3RDPARTY})
    if(${3rdparty_module} STREQUAL "libzmq")
        set(ENABLE_PRECOMPILED OFF
            CACHE BOOL "ENABLE_PRECOMPILED OFF" FORCE)
        set(BUILD_TESTS OFF
            CACHE BOOL "BUILD_TESTS OFF" FORCE)
        set(WITH_PERF_TOOL OFF
            CACHE BOOL "WITH_PERF_TOOL OFF" FORCE)
    endif()

    if(${3rdparty_module} STREQUAL "cppzmq")
        set(CPPZMQ_BUILD_TESTS OFF
            CACHE BOOL "CPPZMQ_BUILD_TESTS OFF" FORCE)
    endif()

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${3rdparty_module})
endforeach(3rdparty_module ${3RDPARTY})

# foreach(module ${MODULES})
#     add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/${module})
# endforeach(module ${MODULES})

foreach(example ${EXAMPLES})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/${example})
endforeach(example ${EXAMPLES})
