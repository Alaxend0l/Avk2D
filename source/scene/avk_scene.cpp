#include "scene/avk_scene.h"
#include "scene/avk_entity.h"
#include "scene/avk_component.h"

namespace avk
{
    AvkScene::AvkScene()
    {
        
    }

    AvkScene::~AvkScene()
    {

    }

    AvkEntity AvkScene::CreateEntity(const std::string& name)
    {
        AvkEntity entity = {registry.create(), this};
        entity.AddComponent<Transform2D>();
        auto& data = entity.AddComponent<EntityData>();
        data.name = name.empty() ? "New Entity" : name;
        return entity;
    }

    void AvkScene::DestroyEntity(AvkEntity entity)
    {
        registry.destroy(entity);
    }

    //template<typename T>
	//void AvkScene::OnComponentAdded(AvkEntity entity, T& component)
	//{
	//	//static_assert(false);
	//}

    AvkEntity AvkScene::GetPrimaryCameraEntity()
	{
        /*
		auto view = registry.view<AvkCamera>();
		for (auto entity : view)
		{
			const auto& camera = view.get<AvkCamera>(entity);
            return AvkEntity{entity, this};
				
		}
        */
		return {};
        
	}
}