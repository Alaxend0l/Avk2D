#pragma once

#include "third_party/entt/entt.hpp"

namespace avk
{
    class AvkEntity;

    class AvkScene
    {
    public:
        AvkScene();
        ~AvkScene();

        AvkEntity CreateEntity(const std::string& name);
        void DestroyEntity(AvkEntity entity);

        //void OnUpdateRuntime(float dt);
		//void OnUpdateEditor(float dt, EditorCamera& camera);
		//void OnViewportResize(uint32_t width, uint32_t height);

        AvkEntity GetPrimaryCameraEntity();

        entt::registry& GetRegistry() {return registry;}

    private:
        //template<typename T>
		//void OnComponentAdded(AvkEntity entity, T& component);

    private:
        entt::registry registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        AvkEntity* selectedEntity;

        friend class AvkEntity;
        friend class AvkLayer_Hierarchy;
    };


}