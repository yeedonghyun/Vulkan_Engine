#include "first_app.hpp"

#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <../glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>

namespace lve {
    FirstApp::FirstApp() {
        loadgameObject();
    }

    FirstApp::~FirstApp() {  }

    //매 프레임 실행
    void FirstApp::run() {

        SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.getSwapChainRenderPass() };

        while (!lveWindow.shouldClose()) {
            glfwPollEvents();
            if (auto commandBuffer = lveRenderer.beginFrame()) {
                lveRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObject);
                lveRenderer.endSwapChainRenderPass(commandBuffer);
                lveRenderer.endFrame();
            }
        }
        vkDeviceWaitIdle(lveDevice.device());
    }

    //모델을 로드
    //현재는 삼각형 및 색상지정
    void FirstApp::loadgameObject() {
        std::vector<LveModel::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}} };

        auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

        auto triangle = LveGameObject::createGameObject();
        triangle.model = lveModel;
        triangle.color = { 0.1f, 0.8f, 0.1f };
        triangle.transform2d.translation.x = 0.2f;
        triangle.transform2d.scale = { 2.f, 0.5f };
        triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

        gameObject.push_back(std::move(triangle));
    }
}