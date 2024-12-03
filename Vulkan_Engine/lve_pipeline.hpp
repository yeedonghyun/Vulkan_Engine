#pragma once
#include "lve_device.hpp"

#include <string>
#include <vector>

namespace lve {

	struct PipelineConfigInfo {
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;

		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;

		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class LvePipeline {
	public:
		//shader ��� �޾Ƽ� shader ���� �а� �׽�Ʈ �ϰ� ����̽� �ʱ�ȭ
		LvePipeline(
			LveDevice& device,
			const std::string& vertFilepath,
			const std::string& fragFilepath,
			const PipelineConfigInfo& configInfo);
		~LvePipeline();

		//���ü��� ����
		LvePipeline(const LvePipeline&) = delete;
		LvePipeline operator=(const LvePipeline&) = delete;
		LvePipeline() = default;

		void bind(VkCommandBuffer commandBuffer);

		//PipelineConfigInfo �����ϰ� Pipeline ���� ����
		static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
		 
	private:
		//������ �а� �����ߴٸ� vector<char> ���� ������ ����
		//�����ߴٸ� throw 
		static std::vector<char> readFile(const std::string& filePath);

		//shader ���� �д� �Լ�(������ �а� �׽�Ʈ �ϴ� �κи� ������)
		void createGraphicsPipeline(
			const std::string& vertFilepath,
			const std::string& fragFilepath,
			const PipelineConfigInfo& configInfo);

		//shader module ����
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		LveDevice& lveDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}