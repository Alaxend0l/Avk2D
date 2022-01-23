#pragma once

#include "avk_pipeline.h"
#include "avk_device.h"
#include "avk_entity.h"

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

        void renderEntities(VkCommandBuffer commandBuffer, std::vector<AvkEntity> &entities);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);
		
		AvkDevice& avkDevice;

		std::unique_ptr<AvkPipeline> avkPipeline;
		VkPipelineLayout pipelineLayout;
	};
}