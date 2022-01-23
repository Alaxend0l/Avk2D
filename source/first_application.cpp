#include "first_application.h"
#include "simple_render_system.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std

#include <stdexcept>
#include <array>

namespace avk
{

	FirstApplication::FirstApplication()
	{
		loadEntities();
	}

	FirstApplication::~FirstApplication()
	{
	}

	void FirstApplication::run()
	{
		SimpleRenderSystem simpleRenderSystem{avkDevice, avkRenderer.getSwapChainRenderPass()};

		while (!avkWindow.shouldClose())
		{
			// Polls events like keystrokes
			glfwPollEvents();

			// Draw game using avkRenderer
			if (auto commandBuffer = avkRenderer.beginFrame())
			{
				// begin offscreen shadow pass
				// render shadow casting objects
				// end offscreen shadow pass
				
				avkRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderEntities(commandBuffer, entities);
				avkRenderer.endSwapChainRenderPass(commandBuffer);
				avkRenderer.endFrame();
			}
		}

		// Once game is closing, wait to close until devices are idle.
		vkDeviceWaitIdle(avkDevice.device());
	}

	void FirstApplication::loadEntities()
	{
		std::vector<AvkModel::Vertex> vertices {};

		vertices.push_back({ {-0.5f, 0.5f} , {1.0f, 0.0f, 0.0f} });
		vertices.push_back({ {0.5f, 0.5f} , {0.0f, 1.0f, 0.0f} });
		vertices.push_back({ {0.0f, -0.5f} , {0.0f, 0.0f, 1.0f} });

		auto avkModel = std::make_shared <AvkModel> (avkDevice, vertices);

		auto triangle = AvkEntity::createEntity();
		triangle.model = avkModel;
		triangle.color = {.1f, .8f, .1f};
		triangle.transform2d.translation.x = .2;
		triangle.transform2d.scale = {2.f, 0.5f};
		triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

		entities.push_back(std::move(triangle));
	}
}