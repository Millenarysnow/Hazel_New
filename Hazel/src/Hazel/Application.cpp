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
			for (Layer* layer : m_layer_stack)
				layer->OnUpdate();
			
			m_window->OnUpdate();
		}
	}

    void Application::OnEvent(Event &e)
    {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		HZ_CORE_TRACE("{0}", e);

		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin(); )
		{
			-- it;
			(*it)->OnEvent(e);

			if (e.GetHandled()) break;
		}
    }

    void Application::PushLayer(Layer* layer)
    {
		m_layer_stack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay)
    {
		m_layer_stack.PushOverlay(overlay);
    }

    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
		m_running = false;
        return true;
    }
}
