#include "lve_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace lve {

    //shader ��� �޾Ƽ� shader ���� �а� �׽�Ʈ
    LvePipeline::LvePipeline(LveDevice& device, 
        const std::string& vertFilePath, 
        const std::string& fragFilePath,
        const PipeLindeConfigInfo configInfo) : lveDevice(device) {
        createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
    }


    //������ �а� �����ߴٸ� vector<char> ���� ������ ����
    //�����ߴٸ� throw 
    std::vector<char> LvePipeline::readFile(const std::string& filepath) {
        std::ifstream file{ filepath, std::ios::ate | std::ios::binary };
        if (!file.is_open()) {
            throw std::runtime_error("failed to open file: " + filepath);
        }
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }

    //shader ���� �д� �Լ�(������ �а� �׽�Ʈ �ϴ� �κи� ������)
    void LvePipeline::createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipeLindeConfigInfo configInfo) {
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);
        std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
        std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
    }

    //shader module ����
    void LvePipeline::createShaderModule(const std::vector<char> code, VkShaderModule* shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create Shader Module: ");
        }
    }

    //�ʹ��̶� PipeLindeConfigInfo �����ϰ� PipeLindeConfigInfo���� (���߿� �ȿ��� �������� �߰�)
    PipeLindeConfigInfo LvePipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipeLindeConfigInfo configInfo{};
        return configInfo;
    }

}