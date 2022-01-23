#pragma once

#include "core/avk_pipeline.h"
#include "core/avk_device.h"
#include "core/avk_model.h"
#include "scene/avk_entity.h"
#include "scene/avk_component.h"

// std

#include <memory>
#include <vector>

namespace avk
{
	class SimpleRenderSystem
	{
	public:

		SimpleRenderSystem(AvkDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

        void RenderScene(VkCommandBuffer commandBuffer, AvkScene& scene);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);
		
		AvkDevice& avkDevice;

		std::unique_ptr<AvkPipeline> avkPipeline;
		VkPipelineLayout pipelineLayout;
	};
}