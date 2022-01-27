#pragma once

#include "core/avk_device.h"
#include "core/avk_window.h"
#include "imgui/avk_layer.h"

// libs
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

// std
#include <stdexcept>
#include <vector>

// This whole class is only necessary right now because it needs to manage the descriptor pool
// because we haven't set one up anywhere else in the application, and we manage the
// example state, otherwise all the functions could just be static helper functions if you prefered
namespace avk {

static void check_vk_result(VkResult err) {
  if (err == 0) return;
  fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
  if (err < 0) abort();
}

class AvkImgui {
 public:
  AvkImgui(AvkWindow &window, AvkDevice &device, VkRenderPass renderPass, uint32_t imageCount);
  ~AvkImgui();

  void newFrame();

  void update(float dt);
  void render(VkCommandBuffer commandBuffer);
  void AddLayer(AvkLayer* layer);
  void RemoveWindow(AvkLayer* layer);

 private:
  AvkDevice &avkDevice;

  std::vector<AvkLayer*> windows;
  void ShowDockSpace(bool* p_open);
  void SetStyle();
  void DeleteWindows();

  // We haven't yet covered descriptor pools in the tutorial series
  // so I'm just going to create one for just imgui and store it here for now.
  // maybe its preferred to have a separate descriptor pool for imgui anyway,
  // I haven't looked into imgui best practices at all.
  VkDescriptorPool descriptorPool;
  unsigned int currentLayerIndex = 0;
};
}  // namespace avk