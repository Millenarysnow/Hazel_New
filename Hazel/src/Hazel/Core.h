#pragma once

// 修复fmt库的链接错误，使得fmt库可以以头文件形式工作（非视频内的操作）
#define FMT_HEADER_ONLY

#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)
	#else
		#define HAZEL_API __declspec(dllimport)
	#endif // HZ_BUILD_DLL

#endif // HZ_PLATFORM_WINDOWS

#ifdef HZ_DEBUG
	#define HZ_ENABLE_ASSERTS
#endif

#ifdef HZ_ENABLE_ASSERTS
	// 目前仅支持 Windwos 平台，断言失败会log并添加断点
	#define HZ_ASSERT(x, ...) { if(!(x)) {HZ_ERROR("Assertion Faild : {0}", __VA_ARGS__); __debugbreak(); }}
	#define HZ_CORE_ASSERT(x, ...) { if(!(x)) {HZ_CORE_ERROR("Assertion Faild : {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif // HZ_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
