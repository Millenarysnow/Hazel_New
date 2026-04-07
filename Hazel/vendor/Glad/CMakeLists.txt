project(Glad LANGUAGES C CXX)

add_library(Glad STATIC
    include/glad/glad.h
    include/KHR/khrplatform.h
    src/glad.c
)

target_include_directories(Glad PUBLIC include)

# Because we are building Glad as a static library that gets linked
# to a DLL (Hazel) which might be built with multithreaded CRT depending on MSVC configs
# But CMake will automatically handle it according to the root CMakeLists.txt CMAKE_MSVC_RUNTIME_LIBRARY policy
