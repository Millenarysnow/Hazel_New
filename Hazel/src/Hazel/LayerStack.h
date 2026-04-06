#pragma once

#include "Hazel/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel {
    
    /**
     * 维护所有 Layer 的栈
     * 渲染顺序自底向上（自前向后），事件处理反向
     * 栈中底部为普通层，覆盖层固定位于栈的顶部，栈保证了这点
     */
    class HAZEL_API LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_layers.end(); }

    private:
        std::vector<Layer*> m_layers;
        std::vector<Layer*>::iterator m_layer_insert; // 普通层插入位置迭代器
    };
}
