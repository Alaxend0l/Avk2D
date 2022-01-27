#pragma once

#include "core/avk_camera.h"

// lib

#include <vulkan/vulkan.h>

namespace avk
{
    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        AvkCamera& camera;
        VkDescriptorSet globalDescriptorSet;
    };
}