#pragma once

#include "Hazel/Core.h"
#include "spdlog/fmt/ostr.h"

#include <string>
#include <functional>

namespace Hazel {

	enum class EventType // 事件类型
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory // 事件过滤
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput	     = BIT(1),
		EventCategoryKeyboard	 = BIT(2),
		EventCategoryMouse	     = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
							   virtual EventType GetEventType() const override { return GetStaticType(); }\
							   virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }
	
	class HAZEL_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	protected:
		bool m_Handled = false; // 是否处理，用于在事件分发向下一层时可以截断分发
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>; // 这里的EventFn是一个接收T&作为参数，返回bool的函数

	public:
		EventDispatcher(Event& event)
			: m_Event(event) { }

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				// 这里func(*(T*)&m_Event)是一个C风格的转换
				// &m_Event取到地址，类型为Event*，然后强制转换为(T*)的指针
				// 接着对(T*)类型的指针解引用，得到T&，传给EventFn函数
				return true;
			}
			return false;
		}
		
	private:
		Event& m_Event;
	};

	// 重载<<操作符，方便配合spdlog打印输出
	// inline std::ostream& operator<<(std::ostream& os, const Event& e)
	// {
	// 	return os << e.ToString();
	// }
	inline std::string format_as(const Event& e) { return e.ToString(); }
}
