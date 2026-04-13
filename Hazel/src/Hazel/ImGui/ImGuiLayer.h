#pragma once

#include "Hazel/Layer.h"


namespace Hazel {

    class MouseButtonPressedEvent;
    class MouseButtonReleasedEvent;
    class WindowResizeEvent;
    class KeyReleasedEvent;
    class KeyPressedEvent;
    class MouseScrolledEvent;
    class MouseMovedEvent;
    class KeyTypedEvent;
    
    
    class HAZEL_API ImGuiLayer : public Layer
    {   
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;

        void OnUpdate() override;
        void OnEvent(Event& event) override;

    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrollEvent(MouseScrolledEvent& e);
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyTypedEvent(KeyTypedEvent& e);
        bool OnWindowsResizedEvent(WindowResizeEvent& e);

    private:
        float m_time = 0.0f;
    };

}
