#include "avk_renderer.h"

// std

#include <stdexcept>
#include <array>

namespace avk
{

	AvkRenderer::AvkRenderer(AvkWindow& window, AvkDevice& device) : avkWindow{window}, avkDevice{device}
	{
		recreateSwapChain();
		createCommandBuffers();
	}

	AvkRenderer::~AvkRenderer()
	{
		freeCommandBuffers();
	}

	void AvkRenderer::createCommandBuffers()
	{
		commandBuffers.resize(AvkSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = avkDevice.getCommandPool();

		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(avkDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers!");
		}

	}

	void AvkRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(avkDevice.device(), avkDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	void AvkRenderer::recreateSwapChain()
	{

		auto extent = avkWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = avkWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(avkDevice.device());

		if (avkSwapChain == nullptr)
		{
			avkSwapChain = std::make_unique<AvkSwapChain>(avkDevice, extent);
		}
		else
		{
			std::shared_ptr<AvkSwapChain> oldSwapChain = std::move(avkSwapChain);
			avkSwapChain = std::make_unique<AvkSwapChain>(avkDevice, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*avkSwapChain.get()))
			{
				throw std::runtime_error("Swap chain image or depth format has changed!");
			}
		}

		// We'll come back here later!
	}

    VkCommandBuffer AvkRenderer::beginFrame()
    {
        assert(!isFrameStarted && "Can't call beginFrame while already in progress!");

		auto result = avkSwapChain->acquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

        return commandBuffer;
    }

    void AvkRenderer::endFrame()
    {
        assert (isFrameStarted && "Can't call endFrame while frame is not in progress!");

        auto commandBuffer = getCurrentCommandBuffer();

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to record command buffer!");
		}

        auto result = avkSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || avkWindow.wasWindowResized())
		{
			avkWindow.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swap chain image!");
		}

        isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % AvkSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void AvkRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert (isFrameStarted && "Can't call beginSwapChainRenderPass while frame is not in progress!");
        assert (commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame!");

        VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = avkSwapChain->getRenderPass();
		renderPassInfo.framebuffer = avkSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = avkSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f,0.01f,0.01f,1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(avkSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(avkSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, avkSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void AvkRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert (isFrameStarted && "Can't call endSwapChainRenderPass while frame is not in progress!");
        assert (commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame!");
        vkCmdEndRenderPass(commandBuffer);
    }
}