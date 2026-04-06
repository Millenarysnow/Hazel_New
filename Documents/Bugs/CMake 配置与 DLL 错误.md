# CMake 配置与 DLL 错误

## 1. 问题代码位置和问题代码、修复后代码

问题文件：`D:/CppProgram/Hazel_New/CMakeLists.txt`

问题位置：`if(MSVC)` 这一段的 MSVC Runtime Library 配置。

问题代码：

```cmake
# 对应 Premake 的 staticruntime "On"
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_compile_options(/utf-8) # 使用 UTF-8（spdlog要求）
endif()
```

修复后代码：

```cmake
# Hazel 目前按 DLL 拆分为 Hazel + Sandbox，必须共享 MSVC DLL CRT，
# 否则会出现跨模块 new/delete 导致的 Debug Heap 断言。
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    add_compile_options(/utf-8) # 使用 UTF-8（spdlog要求）
endif()
```

相关调用链：

- `Sandbox/src/SandboxApp.cpp`

```cpp
Sandbox()
{
    PushLayer(new ExampleLayer());
}
```

- `Hazel/src/Hazel/LayerStack.cpp`

```cpp
LayerStack::~LayerStack()
{
    for (Layer* layer : m_layers)
        delete layer;
}
```

- `Hazel/src/Hazel/EntryPoint.h`

```cpp
auto app = Hazel::CreateApplication();
app->Run();
delete app;
```

## 2. 问题分析和原因

当前工程中：

- `Hazel` 被编译为 `SHARED` 动态库（`Hazel.dll`）
- `Sandbox` 是可执行程序，并链接 `Hazel.dll`
- `ExampleLayer` 在 `Sandbox` 模块中通过 `new` 分配
- `LayerStack::~LayerStack()` 在 `Hazel.dll` 中通过 `delete` 释放

原来的 CMake 配置使用的是：

- Debug: `/MTd`
- Release: `/MT`

也就是静态 CRT（C Runtime）。

在这种配置下，`Sandbox` 和 `Hazel.dll` 会各自带一份独立的运行库堆。这样就会出现“在哪个模块分配，就必须在哪个模块释放”的限制。

但当前代码的实际行为是：

- 在 `Sandbox` 中 `new ExampleLayer()`
- 在 `Hazel.dll` 中 `delete layer`

这属于跨模块释放内存。对于使用静态 CRT 的 DLL/EXE 组合，这是典型的未定义行为。在 Debug 模式下，MSVC 常见表现就是程序运行看似正常，但在关闭窗口、析构对象时触发 CRT 的堆断言，例如：

```text
debug_heap.cpp
Expression: __acrt_first_block == header
```

修复方法是让 `Sandbox` 和 `Hazel.dll` 使用同一套 DLL CRT：

- Debug: `/MDd`
- Release: `/MD`

对应到 CMake，就是把：

```cmake
MultiThreaded$<$<CONFIG:Debug>:Debug>
```

改成：

```cmake
MultiThreaded$<$<CONFIG:Debug>:Debug>DLL
```

修改后，`Sandbox` 和 `Hazel.dll` 共享同一套 MSVC 运行库，跨模块 `new/delete` 不再落到不同的堆上，程序关闭时的 Debug Heap 断言也就消失了。

## 结论

这次错误是 DLL 工程下 CMake 运行库配置错误导致的内存释放跨堆问题。
