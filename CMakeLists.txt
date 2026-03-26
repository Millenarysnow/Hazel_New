cmake_minimum_required(VERSION 3.21)

# 如果你机器上确实固定使用这个 Windows SDK，可以保留。
# 如果你希望换机器更省心，也可以删掉这行，让 VS / CMake 自己选已安装的 SDK。
if(WIN32)
    set(CMAKE_SYSTEM_VERSION 10.0.22621.0)
endif()

project(Hazel LANGUAGES CXX)

# 如果你想在 VS Code 里获得更好的 C++ 工具体验（尤其是 clangd / 现代补全），
# 推荐配合 Ninja / Ninja Multi-Config 使用这个选项。
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 支持 Debug / Release / Dist
get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(IS_MULTI_CONFIG)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist" CACHE STRING "" FORCE)
else()
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "Dist")
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
    endif()
endif()

# 让 Dist 默认沿用 Release 的优化/链接参数
set(CMAKE_C_FLAGS_DIST "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DIST "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_DIST "${CMAKE_EXE_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_DIST "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}" CACHE STRING "" FORCE)

if(NOT WIN32)
    message(FATAL_ERROR "当前工程仅配置了 Windows 平台。")
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "当前工程仅支持 x64，请使用 x64 工具链/生成器。")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 对应 Premake 的 staticruntime "On"
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    # 如果你以后想启用 /utf-8，可以取消下一行注释
    # add_compile_options(/utf-8)
endif()

# 尽量保持和你现在 Premake 一样的最终输出结构
set(OUTPUT_ROOT "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>-windows-x64")

# 注意：
# CMake 官方更推荐手写源文件列表，而不是 GLOB。
# 但为了和你现在 Premake 的“自动抓取 src 下所有 .h/.cpp”行为保持接近，
# 这里使用 GLOB_RECURSE + CONFIGURE_DEPENDS。
file(GLOB_RECURSE HAZEL_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Hazel/src/*.h"
    "${CMAKE_SOURCE_DIR}/Hazel/src/*.cpp"
)

file(GLOB_RECURSE SANDBOX_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Sandbox/src/*.h"
    "${CMAKE_SOURCE_DIR}/Sandbox/src/*.cpp"
)

# =========================
# Hazel (Shared Library)
# =========================
add_library(Hazel SHARED ${HAZEL_FILES})

target_include_directories(Hazel
    PUBLIC
        "${CMAKE_SOURCE_DIR}/Hazel/src"
        "${CMAKE_SOURCE_DIR}/Hazel/vendor/spdlog/include"
)

target_compile_definitions(Hazel
    PRIVATE
        HZ_PLATFORM_WINDOWS
        HZ_BUILD_DLL
        $<$<CONFIG:Debug>:HZ_DEBUG>
        $<$<CONFIG:Release>:HZ_RELEASE>
        $<$<CONFIG:Dist>:HZ_DIST>
)

set_target_properties(Hazel PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_ROOT}/Hazel"
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_ROOT}/Hazel"
    ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_ROOT}/Hazel"
)

# =========================
# Sandbox (Console App)
# =========================
add_executable(Sandbox ${SANDBOX_FILES})

target_include_directories(Sandbox
    PRIVATE
        "${CMAKE_SOURCE_DIR}/Sandbox/src"
)

target_link_libraries(Sandbox PRIVATE Hazel)

target_compile_definitions(Sandbox
    PRIVATE
        HZ_PLATFORM_WINDOWS
        $<$<CONFIG:Debug>:HZ_DEBUG>
        $<$<CONFIG:Release>:HZ_RELEASE>
        $<$<CONFIG:Dist>:HZ_DIST>
)

set_target_properties(Sandbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_ROOT}/Sandbox"
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_ROOT}/Sandbox"
    ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_ROOT}/Sandbox"
)

# 对应 Premake 里的 postbuildcommands：
# 把 Hazel.dll 复制到 Sandbox 的输出目录
add_custom_command(TARGET Hazel POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Sandbox>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Hazel>
            "$<TARGET_FILE_DIR:Sandbox>"
)

# 如果你用 Visual Studio 生成器，这样打开时默认启动项目是 Sandbox
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)