#pragma once
#include "lve_device.hpp"

#include <string>
#include <vector>

namespace lve {

	struct PipelineConfigInfo {
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class LvePipeline {
	public:
		//shader 경로 받아서 shader 파일 읽고 테스트 하고 디바이스 초기화
		LvePipeline(LveDevice&device, const std::string &vertFilePath, 
			const std::string& fragFilePath, const PipelineConfigInfo configInfo);
		~LvePipeline();

		//동시선언 방지
		LvePipeline(const LvePipeline&) = delete;
		void operator=(const LvePipeline&) = delete;

		void bind(VkCommandBuffer commandBuffer);

		//PipelineConfigInfo 생성하고 Pipeline 구성 설정
		static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		//파일을 읽고 성공했다면 vector<char> 파일 정보를 리턴
		//실패했다면 throw 
		static std::vector<char> readFile(const std::string& filePath);

		//shader 파일 읽는 함수(파일을 읽고 테스트 하는 부분만 구현됨)
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo configInfo);

		//shader module 생성
		void createShaderModule(const std::vector<char> code, VkShaderModule* shaderModule);

		LveDevice& lveDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}