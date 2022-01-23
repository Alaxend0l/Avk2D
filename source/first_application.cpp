#include "first_application.h"
#include "render/simple_render_system.h"
#include "scene/avk_component.h"

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
		avkScene = std::make_shared<AvkScene>();

		LoadEntities(avkScene);
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
				simpleRenderSystem.RenderScene(commandBuffer, *avkScene);
				avkRenderer.endSwapChainRenderPass(commandBuffer);
				avkRenderer.endFrame();
			}
		}

		// Once game is closing, wait to close until devices are idle.
		vkDeviceWaitIdle(avkDevice.device());
	}

	void FirstApplication::LoadEntities(std::shared_ptr<AvkScene>& scene)
	{
		std::vector<AvkModel::Vertex> vertices {};

		vertices.push_back({ {-0.5f, 0.5f} , {1.0f, 0.0f, 0.0f} });
		vertices.push_back({ {0.5f, 0.5f} , {0.0f, 1.0f, 0.0f} });
		vertices.push_back({ {0.0f, -0.5f} , {0.0f, 0.0f, 1.0f} });

		auto avkModel = std::make_shared <AvkModel> (avkDevice, vertices);

		auto triangle = scene->CreateEntity("Triangle");
		auto &transform = triangle.GetComponent<Transform2D>();
		triangle.AddComponent<std::shared_ptr<AvkModel>>(avkModel);
		//triangle.color = {.1f, .8f, .1f};
		transform.position.x = .2;
		transform.scale = {2.f, 0.5f};
		transform.rotation = 0.25f * glm::two_pi<float>();
	}
}