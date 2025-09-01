workspace "Hazel"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" -- 标记输出位置（使用预定义的宏）

project "Hazel"
	location "Hazel" -- 指明目录
	kind "SharedLib" -- 指明为动态库
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") -- 目标文件输出目录
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}") -- 临时文件输出目录

	files -- 源文件目录
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs -- 包含文件目录
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src"
	}

	-- buildoptions "/utf-8"

	--[[
		定义过滤器：直到遇到另一个过滤器之前都是该过滤器范围
		过滤器不会自动嵌套，如果需要同时满足两种条件，必须在一个过滤器中全部写明
		定义一个多条件过滤器：
		filter { [条件1], [条件2], ...}
			...
	]]--

	filter "system:windows" -- windows
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.22621.0" -- SDK版本

		defines -- 预定义宏
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL"
		}

		postbuildcommands -- 后处理命令（复制dll至正确的位置）
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		optimize "On"
		
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") -- 目标文件输出目录
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}") -- 临时文件输出目录

	files -- 源文件目录
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs -- 包含文件目录
	{
		"Hazel/vendor/spdlog/include",
		"Hazel/src" -- 依赖Hazel暴露出的指定头文件
	}

	links -- 链接
	{
		"Hazel"
	}

	-- buildoptions "/utf-8"

	filter "system:windows" -- windows
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.22621.0" -- SDK版本

		defines -- 预定义宏
		{
			"HZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		optimize "On"