#include "Application.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"

namespace Hazel {
	
	Application::Application()
	{
		m_window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_running)
		{
			m_window->OnUpdate();
		}
	}

}