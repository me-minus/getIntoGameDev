#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>
#include <vector>

class PushConstants
{
 public:
  PushConstants();

  std::vector<vk::PushConstantRange>& getRanges();
  void update_value();

  void record_draw_commands(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout,vk::DispatchLoaderDynamic dl);
 private:
  float counter;
  std::vector<vk::PushConstantRange> pushConstantRanges;
};
