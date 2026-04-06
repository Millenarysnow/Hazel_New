#pragma once

#include "Hazel/core.h"
#include "Hazel/Events/Event.h"

namespace Hazel{

    /**
     * Layer 是一种抽象层
     * Layer 生命周期与 Application 相同，从栈中清除并不会直接销毁
     * Layer 在栈中自底向上渲染
     * Layer 在栈中分为两类，其中覆盖层将被保证永远位于栈顶
     */
    class HAZEL_API Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() { } // 附加到栈中
        virtual void OnDetach() { } // 从栈中移除
        virtual void OnUpdate() { }
        virtual void OnEvent(Event& event) { }

        inline const std::string& GetName() const { return m_debug_name; }
        
    protected:
        std::string m_debug_name;
    };

}