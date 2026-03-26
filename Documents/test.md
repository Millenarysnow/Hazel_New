```mermaid
classDiagram
    %% --- 1. 枚举定义 ---
    class EventType {
        <<enumeration>>
        WindowClose
        WindowResize
        KeyPressed
        KeyReleased
        MouseButtonPressed
        MouseButtonReleased
        MouseMoved
        MouseScrolled
    }

    class EventCategory {
        <<enumeration>>
        None
        EventCategoryApplication
        EventCategoryInput
        EventCategoryKeyboard
        EventCategoryMouse
        EventCategoryMouseButton
    }

    %% --- 2. 核心类 ---
    class Application {
        +Run()
        +CreateApplication()$ Application*
    }

    class Log {   
        -s CoreLogger : shared_ptr~logger~ 
        -s ClientLogger : shared_ptr~logger~
        +Init()
        +GetCoreLogger() shared_ptr~logger~
        +GetClientLogger() shared_ptr~logger~
    }

    %% --- 3. 事件系统 ---
    class Event {
        <<abstract>>
        #m_Handled : bool
        +GetEventType() *EventType
        +GetName()* const char*
        +GetCategoryFlags()* int
        +IsInCategory(category) bool
        +ToString() string
    }

    class EventDispatcher {
        -m_Event : Event&
        +Dispatch~T~(func) bool
    }

    %% --- 4. 具体事件 ---
    class KeyEvent {
        #m_KeyCode : int
        #KeyEvent(keycode)
        +GetKeyCode() int
    }

    class KeyPressedEvent {
        -m_RepeatCount : int
        +KeyPressedEvent(keycode, repeat)
        +GetRepeatCount() int
        +ToString() string
    }

    class KeyReleasedEvent {
        +KeyReleasedEvent(keycode)
        +ToString() string
    }

    %% --- 5. 关系连线 ---
    %% 继承
    Event <|-- KeyEvent
    KeyEvent <|-- KeyPressedEvent
    KeyEvent <|-- KeyReleasedEvent

    %% 关联
    %% EventDispatcher 持有 Event 的引用
    EventDispatcher --> Event : holds reference

    %% 依赖
    Event ..> EventType : uses
    Event ..> EventCategory : uses
```

