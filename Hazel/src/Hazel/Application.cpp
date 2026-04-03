#include "Application.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"

namespace Hazel {
	
// bind 是什么?
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1) 

	Application::Application()
	{
		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));
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

    void Application::OnEvent(Event &e)
    {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		HZ_CORE_TRACE("{0}", e);
    }

    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
		m_running = false;
        return true;
    }
}
