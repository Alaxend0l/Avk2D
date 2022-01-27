#pragma once

// std

#include <string>

namespace avk
{
    class AvkLayer
    {
    public:
        AvkLayer(const std::string& name = "Layer");
        virtual ~AvkLayer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float dt) {}
        virtual void OnImGuiRender() {}

        const std::string& GetName() const { return layerName; }

    protected:
        std::string layerName;
        bool show{true};
    };
}