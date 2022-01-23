#pragma once

#include "avk_model.h"

// std

#include <memory>

namespace avk
{
    struct Transform2D
    {
        glm::vec2 translation{};
        glm::vec2 scale{1.f, 1.f};
        float rotation;
        glm::mat2 mat2()
        {
            const float sin = glm::sin(rotation);
            const float cos = glm::cos(rotation);

            glm::mat2 rotMatrix{{cos,sin},{-sin,cos}};
            glm::mat2 scaleMatrix{{scale.x, 0},{0,scale.y}};

            return rotMatrix * scaleMatrix;
        }
    };

    class AvkEntity {
    public:
        using id_t = unsigned int;

        static AvkEntity createEntity()
        {
            static id_t currentId = 0;
            return AvkEntity{currentId++};
        }

        AvkEntity(const AvkEntity&) = delete;
		AvkEntity& operator=(const AvkEntity&) = delete;
        AvkEntity(AvkEntity&&) = default;
		AvkEntity& operator=(AvkEntity&&) = default;

        const id_t getId() { return id; }

        std::shared_ptr<AvkModel> model{};
        glm::vec3 color{};

        Transform2D transform2d{};

    private:
        AvkEntity(id_t entityId) : id{entityId} {}

        id_t id;

    };
}