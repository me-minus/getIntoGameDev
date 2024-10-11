#include "pushconstants.h"
#include <cmath>

#include <iostream>

PushConstants::PushConstants(): counter(0)
{
  vk::PushConstantRange pushConstantInfo;
  pushConstantInfo.offset = 0;
  pushConstantInfo.size = sizeof(float);
  pushConstantInfo.stageFlags = vk::ShaderStageFlagBits::eVertex;

  pushConstantRanges.push_back(pushConstantInfo);
}


std::vector<vk::PushConstantRange>& PushConstants::getRanges()
{
  return pushConstantRanges;
}

void PushConstants::update_value()
{
  counter+=1.0f;
}


void PushConstants::record_draw_commands(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, vk::DispatchLoaderDynamic dl)
{
  float pushValues{std::fabs(std::cosf(counter*std::numbers::pi/180))};

  vk::ShaderStageFlags stageFlags(vk::ShaderStageFlagBits::eVertex);
  commandBuffer.pushConstants(pipelineLayout,stageFlags,0,sizeof(pushValues),&pushValues,dl);  
}
