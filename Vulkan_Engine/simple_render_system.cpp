#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <../glm/glm.hpp>
#include <../glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <stdexcept>

namespace lve {

    //오브젝트 위치, 회전, 크기
    struct SimplePushConstantData {
        glm::mat2 transform{ 1.f };
        glm::vec2 offset;
        //16바이트 정렬
        alignas(16) glm::vec3 color; 
    };

    SimpleRenderSystem::SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass)
        : lveDevice{ device } {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem() {
        vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
    }

    //파이프라인 생성
    void SimpleRenderSystem::createPipelineLayout() {
        //상수 버퍼 설정
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);
        
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        //상수 버퍼 연결
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        
        //기본 파이프라인 설정 불러오기
        LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;

        //파이프라인 생성
        lvePipeline = std::make_unique<LvePipeline>(
            lveDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig);
    }

    void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<LveGameObject>& gameObjects) {
        //파이프라인 활성화
        lvePipeline->bind(commandBuffer);

        //오브젝트별 렌더링
        for (auto& obj : gameObjects) {
            obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.0001f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            push.transform = obj.transform2d.mat2();

            //상수 버퍼를 GPU에 전달
            vkCmdPushConstants(
                commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);

            //오브젝트 데이터 바인딩
            obj.model->bind(commandBuffer);
            //오브젝트 그리기
            obj.model->draw(commandBuffer);
        }
    }
}