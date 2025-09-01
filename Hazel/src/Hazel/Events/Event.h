#pragma once

#include "Hazel/Core.h"

#include <string>
#include <functional>

namespace Hazel {

	enum class EventType // 事件类型
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, keyReleased,
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
		friend class EventDispacher;
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
}

