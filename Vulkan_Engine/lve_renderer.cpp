#include "lve_renderer.hpp"

#include <array>
#include <cassert>
#include <stdexcept>

namespace lve {

    LveRenderer::LveRenderer(LveWindow& window, LveDevice& device)
        : lveWindow{ window }, lveDevice{ device } {
        recreateSwapChain();
        createCommandBuffers();
    }

    LveRenderer::~LveRenderer() { freeCommandBuffers(); }

    void LveRenderer::recreateSwapChain() {
        //창크기를 가져옴
        auto extent = lveWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = lveWindow.getExtent();
            //창크기가 유효할 때까지 이벤트 대기
            glfwWaitEvents(); 
        }
        //GPU 작업 대기
        vkDeviceWaitIdle(lveDevice.device());

        if (lveSwapChain == nullptr) {
            lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent);
        }
        else {
            std::shared_ptr<LveSwapChain> oldSwapChain = std::move(lveSwapChain);
            lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent, oldSwapChain);

            if (!oldSwapChain->compareSwapFormats(*lveSwapChain.get())) {
                throw std::runtime_error("Swap chain image(or depth) format has changed!");
            }
        }
    }

    //Command Buffer 생성 정보 설정 및 생성 
    void LveRenderer::createCommandBuffers() {
        commandBuffers.resize(LveSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = lveDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(lveDevice.device(), &allocInfo, commandBuffers.data()) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    //메모리해제
    void LveRenderer::freeCommandBuffers() {
        vkFreeCommandBuffers(
            lveDevice.device(),
            lveDevice.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
    }

    //SwapChain 이미지를 가져오고 성공하면 렌더링 시작
    VkCommandBuffer LveRenderer::beginFrame() {
        assert(!isFrameStarted && "Can't call beginFrame while already in progress");

        //SwapChain 이미지
        auto result = lveSwapChain->acquireNextImage(&currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        //프레임 시작됨
        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        //command buffer 기록 시작
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
        return commandBuffer;
    }

    void LveRenderer::endFrame() {

        assert(isFrameStarted && "Can't call endFrame while frame is not in progress");

        //command buffer 기록 끝
        auto commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

        //command buffer 명령 제출
        auto result = lveSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        //창크기 변경 또는 에러시 스왑채인 재생성
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lveWindow.wasWindowResized()) {
            lveWindow.resetWindowResizeFlag();
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % LveSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    //Rendering 작업을 위한 RenderPass
    void LveRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
        assert(
            commandBuffer == getCurrentCommandBuffer() &&
            "Can't begin render pass on command buffer from a different frame");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = lveSwapChain->getRenderPass();
        renderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();

        //배경색, 깊이 초기화
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        //뷰포트
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(lveSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(lveSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{ {0, 0}, lveSwapChain->getSwapChainExtent() };
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    //RenderPass 종료
    void LveRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
        assert(
            commandBuffer == getCurrentCommandBuffer() &&
            "Can't end render pass on command buffer from a different frame");
        vkCmdEndRenderPass(commandBuffer);
    }
} 