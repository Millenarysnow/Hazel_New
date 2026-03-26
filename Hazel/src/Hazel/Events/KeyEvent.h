#pragma once

#include "Event.h"

#include <sstream>

namespace Hazel {

    class HAZEL_API KeyEvent : public Event
    {
    public:
        inline int GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        // 构造函数是受保护的，因为这个类是一个基类，不应该被直接创建
        // 而是应该创建它的具体的派生子类
        KeyEvent(int keycode) : m_KeyCode(keycode)
        { }
        
        int m_KeyCode; // 按键键码
    };

    class HAZEL_API KeyPressedEvent : public KeyEvent
    {
    public:
        // 显式调用父类被保护的构造函数
        // repeateCount 按键被按下的次数
        KeyPressedEvent(int keycode, int repeatCount)
            : KeyEvent(keycode), m_RepeatCount(repeatCount) { }

        inline int GetRepeatCount() const { return m_RepeatCount; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_RepeatCount;
    };

    class HAZEL_API KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keycode)
            : KeyEvent(keycode) { }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };
    
}
