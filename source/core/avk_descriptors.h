#pragma once
 
#include "core/avk_device.h"
 
// std
#include <memory>
#include <unordered_map>
#include <vector>
 
namespace avk {
 
class AvkDescriptorSetLayout {
 public:
  class Builder {
   public:
    Builder(AvkDevice &avkDevice) : avkDevice{avkDevice} {}
 
    Builder &addBinding(
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags,
        uint32_t count = 1);
    std::unique_ptr<AvkDescriptorSetLayout> build() const;
 
   private:
    AvkDevice &avkDevice;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
  };
 
  AvkDescriptorSetLayout(
      AvkDevice &avkDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
  ~AvkDescriptorSetLayout();
  AvkDescriptorSetLayout(const AvkDescriptorSetLayout &) = delete;
  AvkDescriptorSetLayout &operator=(const AvkDescriptorSetLayout &) = delete;
 
  VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }
 
 private:
  AvkDevice &avkDevice;
  VkDescriptorSetLayout descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;
 
  friend class AvkDescriptorWriter;
};
 
class AvkDescriptorPool {
 public:
  class Builder {
   public:
    Builder(AvkDevice &avkDevice) : avkDevice{avkDevice} {}
 
    Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
    Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
    Builder &setMaxSets(uint32_t count);
    std::unique_ptr<AvkDescriptorPool> build() const;
 
   private:
    AvkDevice &avkDevice;
    std::vector<VkDescriptorPoolSize> poolSizes{};
    uint32_t maxSets = 1000;
    VkDescriptorPoolCreateFlags poolFlags = 0;
  };
 
  AvkDescriptorPool(
      AvkDevice &avkDevice,
      uint32_t maxSets,
      VkDescriptorPoolCreateFlags poolFlags,
      const std::vector<VkDescriptorPoolSize> &poolSizes);
  ~AvkDescriptorPool();
  AvkDescriptorPool(const AvkDescriptorPool &) = delete;
  AvkDescriptorPool &operator=(const AvkDescriptorPool &) = delete;
 
  bool allocateDescriptor(
      const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;
 
  void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;
 
  void resetPool();
 
 private:
  AvkDevice &avkDevice;
  VkDescriptorPool descriptorPool;
 
  friend class AvkDescriptorWriter;
};
 
class AvkDescriptorWriter {
 public:
  AvkDescriptorWriter(AvkDescriptorSetLayout &setLayout, AvkDescriptorPool &pool);
 
  AvkDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
  AvkDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);
 
  bool build(VkDescriptorSet &set);
  void overwrite(VkDescriptorSet &set);
 
 private:
  AvkDescriptorSetLayout &setLayout;
  AvkDescriptorPool &pool;
  std::vector<VkWriteDescriptorSet> writes;
};
 
}