#include "first_application.h"
#include "render/simple_render_system.h"
#include "scene/avk_component.h"
#include "imgui/avk_imgui.h"
#include "core/avk_buffer.h"
#include "core/avk_frame_info.h"


// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std

#include <stdexcept>
#include <array>
#include <chrono>


namespace avk
{
	struct GlobalUbo
	{
		glm::mat4 projectionView{1.f};
		glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
	};

	FirstApplication::FirstApplication()
	{
		globalPool = AvkDescriptorPool::Builder(avkDevice)
		.setMaxSets(AvkSwapChain::MAX_FRAMES_IN_FLIGHT)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, AvkSwapChain::MAX_FRAMES_IN_FLIGHT)
		.build();
		avkScene = std::make_shared<AvkScene>();

		LoadEntities(avkScene);
	}

	FirstApplication::~FirstApplication()
	{
	}

	void FirstApplication::run()
	{
		std::vector<std::unique_ptr<AvkBuffer>> uboBuffers(AvkSwapChain::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < uboBuffers.size(); i++)
		{
			uboBuffers[i] = std::make_unique<AvkBuffer>(
				avkDevice,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);

			uboBuffers[i]->map();
		}

		auto globalSetLayout = AvkDescriptorSetLayout::Builder(avkDevice)
		.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
		.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(AvkSwapChain::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			AvkDescriptorWriter(*globalSetLayout, *globalPool)
			.writeBuffer(0, &bufferInfo)
			.build(globalDescriptorSets[i]);
		}

		AvkImgui avkImgui{
			avkWindow,
			avkDevice,
			avkRenderer.getSwapChainRenderPass(),
			avkRenderer.getImageCount()};

		//AvkLayer_Hierarchy hierarchyWindow{avkScene};

		SimpleRenderSystem simpleRenderSystem{avkDevice, avkRenderer.getSwapChainRenderPass()};
		AvkCamera camera{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!avkWindow.shouldClose())
		{
			// Polls events like keystrokes
			glfwPollEvents();

			// Get new delta time
			auto newTime = std::chrono::high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			// Update the camera to set the projection matrix based on the window aspect ratio.
			float aspect = avkRenderer.getAspectRatio();

			camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);

			// Draw game using avkRenderer
			if (auto commandBuffer = avkRenderer.beginFrame())
			{
				int frameIndex = avkRenderer.getFrameIndex();
				FrameInfo frameInfo{
					frameIndex,
					deltaTime,
					commandBuffer,
					camera,
					globalDescriptorSets[frameIndex]
				};

				//update
				GlobalUbo ubo{};
				ubo.projectionView = camera.getProjection() * camera.getView();
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				// tell imgui that we're starting a new frame
      			avkImgui.newFrame();

				// begin offscreen shadow pass
				// render shadow casting objects
				// end offscreen shadow pass
				
				avkRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.RenderScene(commandBuffer, *avkScene);
				avkImgui.update(deltaTime);
				DrawMenuBar();
				avkImgui.render(commandBuffer);
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

		vertices.push_back({ {-0.5f, -0.5f} , {1.0f, 0.0f, 0.0f} });
		vertices.push_back({ {0.5f, 0.5f} , {0.0f, 1.0f, 0.0f} });
		vertices.push_back({ {0.5f, -0.5f} , {0.0f, 0.0f, 1.0f} });

		auto avkModel = std::make_shared <AvkModel> (avkDevice, vertices);

		auto triangle = scene->CreateEntity("Triangle");
		auto &transform = triangle.GetComponent<Transform2D>();
		triangle.AddComponent<std::shared_ptr<AvkModel>>(avkModel);
		//triangle.color = {.1f, .8f, .1f};
		// transform.position.x = .2;
		transform.scale = {1.0f, 1.0f};
		// transform.rotation = 0.25f * glm::two_pi<float>();
	}

	void FirstApplication::DrawMenuBar()
	{
		static bool showDemoWindow{false};
		if (showDemoWindow)
		{
			ImGui::ShowDemoWindow();
		}
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Demo", NULL, &showDemoWindow)) {}
				if (ImGui::MenuItem("New")) {}
				if (ImGui::MenuItem("Open", "Ctrl+O")) {}
				if (ImGui::BeginMenu("Open Recent"))
				{
					ImGui::MenuItem("fish_hat.c");
					ImGui::MenuItem("fish_hat.inl");
					ImGui::MenuItem("fish_hat.h");
					if (ImGui::BeginMenu("More.."))
					{
						ImGui::MenuItem("Hello");
						ImGui::MenuItem("Sailor");
						if (ImGui::BeginMenu("Recurse.."))
						{
							ImGui::EndMenu();
						}
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S")) {}
				if (ImGui::MenuItem("Save As..")) {}

				ImGui::Separator();
				if (ImGui::BeginMenu("Options"))
				{
					static bool enabled = true;
					ImGui::MenuItem("Enabled", "", &enabled);
					ImGui::BeginChild("child", ImVec2(0, 60), true);
					for (int i = 0; i < 10; i++)
						ImGui::Text("Scrolling Text %d", i);
					ImGui::EndChild();
					static float f = 0.5f;
					static int n = 0;
					ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
					ImGui::InputFloat("Input", &f, 0.1f);
					ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Colors"))
				{
					float sz = ImGui::GetTextLineHeight();
					for (int i = 0; i < ImGuiCol_COUNT; i++)
					{
						const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
						ImVec2 p = ImGui::GetCursorScreenPos();
						ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
						ImGui::Dummy(ImVec2(sz, sz));
						ImGui::SameLine();
						ImGui::MenuItem(name);
					}
					ImGui::EndMenu();
				}

				// Here we demonstrate appending again to the "Options" menu (which we already created above)
				// Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
				// In a real code-base using it would make senses to use this feature from very different code locations.
				if (ImGui::BeginMenu("Options")) // <-- Append!
				{
					static bool b = true;
					ImGui::Checkbox("SomeOption", &b);
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Disabled", false)) // Disabled
				{
					IM_ASSERT(0);
				}
				if (ImGui::MenuItem("Checked", NULL, true)) {}
				if (ImGui::MenuItem("Quit", "Alt+F4")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
    
	}
}