#pragma once
#include "lve_device.hpp"

#include <string>
#include <vector>

namespace lve {

	struct PipeLindeConfigInfo	{	};

	class LvePipeline {
	public:
		//shader ��� �޾Ƽ� shader ���� �а� �׽�Ʈ �ϰ� ����̽� �ʱ�ȭ
		LvePipeline(LveDevice&device, const std::string &vertFilePath, 
			const std::string& fragFilePath, const PipeLindeConfigInfo configInfo);
		~LvePipeline() {}

		//���ü��� ����
		LvePipeline(const LvePipeline&) = delete;
		void operator=(const LvePipeline&) = delete;

		//�ʹ��̶� PipeLindeConfigInfo �����ϰ� PipeLindeConfigInfo���� (���߿� �ȿ��� �������� �߰�)
		static PipeLindeConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		//������ �а� �����ߴٸ� vector<char> ���� ������ ����
		//�����ߴٸ� throw 
		static std::vector<char> readFile(const std::string& filePath);

		//shader ���� �д� �Լ�(������ �а� �׽�Ʈ �ϴ� �κи� ������)
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipeLindeConfigInfo configInfo);

		//shader module ����
		void createShaderModule(const std::vector<char> code, VkShaderModule* shaderModule);

		LveDevice& lveDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}