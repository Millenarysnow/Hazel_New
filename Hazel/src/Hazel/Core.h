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

#define BIT(x) (1 << x)
