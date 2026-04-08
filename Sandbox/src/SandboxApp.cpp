#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer() : Hazel::Layer("ExampleLayer")
	{ }

	void OnUpdate() override
	{
		HZ_INFO("ExampleLayer::OnUpdate");
	}

	void OnEvent(Hazel::Event& event) override
	{
		HZ_TRACE("{0}", event);
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImGuiLayer());
	}
	
	~Sandbox() override { }
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}