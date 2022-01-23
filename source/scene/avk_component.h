#pragma once


// std

#include <string>

// libs

#include <glm/gtc/matrix_transform.hpp>

namespace avk
{
    struct EntityData
    {
        bool isEnabled;
        std::string name;
    };

    struct Transform2D
    {
        glm::vec2 position{};
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
}