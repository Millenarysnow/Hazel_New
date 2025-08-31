#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	Hazel::Log::GetCoreLogger()->warn("Initialized. ");
	Hazel::Log::GetClientLogger()->info("Initialized. ");

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif // HZ_PLATFORM_WINDOWS
