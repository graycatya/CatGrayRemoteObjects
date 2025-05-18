# CatGrayRemoteObjects

CatGrayRemoteObjects is a lightweight messaging library based on libzmq encapsulation.

## Examples

| Project | Explanation |
|:--:|:--:|
| cppzmq_examples | cppzmq test example |
| gui_examples | CatGrayRemoteObjects interface test example with graphical interface application, need to use imgui |

## Submodule

CmakeLists.txt in the CatGrayRemoteObjects main directory is responsible for introducing the submodules of the src directory, each of which does not directly depend on each other.

### libzmq

The ZeroMQ lightweight messaging kernel is a library which extends the
standard socket interfaces with features traditionally provided by
specialised messaging middleware products. ZeroMQ sockets provide an
abstraction of asynchronous message queues, multiple messaging patterns,
message filtering (subscriptions), seamless access to multiple transport
protocols and more.

### cppzmq

cppzmq is a C++ binding for libzmq.

### imgui-gl3

Dear ImGui is a **bloat-free graphical user interface library for C++**. It outputs optimized vertex buffers that you can render anytime in your 3D-pipeline-enabled application. It is fast, portable, renderer agnostic, and self-contained (no external dependencies).

## Quick-start your own project


* Project submodule initialization.

```
git submodule update --init --recursive
```

* cmake 

```cmake
# cmake
add_subdirectory(CatGrayRemoteObjects)

target_link_libraries(${PROJECT_NAME}
  PRIVATE CatGrayRemoteObjects_a)
```

