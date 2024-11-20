#include "lve_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace lve {

    //shader 경로 받아서 shader 파일 읽고 테스트
    LvePipeline::LvePipeline(LveDevice& device, 
        const std::string& vertFilePath, 
        const std::string& fragFilePath,
        const PipeLindeConfigInfo configInfo) : lveDevice(device) {
        createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
    }


    //파일을 읽고 성공했다면 vector<char> 파일 정보를 리턴
    //실패했다면 throw 
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

    //shader 파일 읽는 함수(파일을 읽고 테스트 하는 부분만 구현됨)
    void LvePipeline::createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipeLindeConfigInfo configInfo) {
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);
        std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
        std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
    }

    //shader module 생성
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

    //초반이라 PipeLindeConfigInfo 생성하고 PipeLindeConfigInfo리턴 (나중에 안에들어갈 정보들을 추가)
    PipeLindeConfigInfo LvePipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipeLindeConfigInfo configInfo{};
        return configInfo;
    }

}