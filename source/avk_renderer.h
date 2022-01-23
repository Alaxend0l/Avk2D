#pragma once

#include "avk_window.h"
#include "avk_device.h"
#include "avk_swap_chain.h"

// std

#include <memory>
#include <vector>
#include <cassert>

namespace avk
{
	class AvkRenderer
	{
	public:

		AvkRenderer(AvkWindow& window, AvkDevice& device);
		~AvkRenderer();

		AvkRenderer(const AvkRenderer&) = delete;
		AvkRenderer& operator=(const AvkRenderer&) = delete;

        VkRenderPass getSwapChainRenderPass() const
        {
            return avkSwapChain->getRenderPass();
        }

        bool isFrameInProgress() const {return isFrameStarted;}

        VkCommandBuffer getCurrentCommandBuffer() const
        {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress!");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const 
        {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress!");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();

        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		AvkWindow& avkWindow;
		AvkDevice& avkDevice;
		std::unique_ptr<AvkSwapChain> avkSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted{false};
	};
}