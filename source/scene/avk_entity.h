#pragma once

#include "scene/avk_scene.h"

#include "third_party/entt/entt.hpp"

namespace avk
{
    class AvkEntity
    {
    public:
        AvkEntity() = default;
		AvkEntity(entt::entity handle, AvkScene* _scene);
		AvkEntity(const AvkEntity& other) = default;

        template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
            //[TODO] Make a replacement for this!
            //HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
			//scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
            //[TODO] Make a replacement for this!
			//HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return scene->registry.get<T>(entityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return scene->registry.try_get<T>(entityHandle) != NULL;
		}

		template<typename T>
		void RemoveComponent()
		{
            //[TODO] Make a replacement for this!
			//HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			scene->registry.remove<T>(entityHandle);
		}

        operator bool() const { return entityHandle != entt::null; }
		operator entt::entity() const { return entityHandle; }
		operator uint32_t() const { return (uint32_t)entityHandle; }

		bool operator==(const AvkEntity& other) const
		{
			return entityHandle == other.entityHandle && scene == other.scene;
		}

		bool operator!=(const AvkEntity& other) const
		{
			return !(*this == other);
		}

    private:
        entt::entity entityHandle{entt::null};
        AvkScene* scene = nullptr;
    };
}