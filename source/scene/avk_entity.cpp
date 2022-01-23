#include "scene/avk_entity.h"

namespace avk
{
    AvkEntity::AvkEntity(entt::entity handle, AvkScene* _scene): entityHandle(handle), scene(_scene) {}
}