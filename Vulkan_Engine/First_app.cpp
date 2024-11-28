#include "first_app.hpp"

#include "keyboard_movement_controller.hpp"
#include "lve_camera.hpp"
#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <../glm/gtc/constants.hpp>

#include <chrono>
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
        LveCamera camera{};

        //카메라의 위치, 바라보는 방향, 카메라의 윗부분의 방향
        camera.setViewTarget(glm::vec3(-1.f, -2.f, -0.2f), glm::vec3(0.f, 0.f, 2.5f));

        auto viewerObject = LveGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!lveWindow.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime =
                std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;


            cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = lveRenderer.getAspectRatio();

            //뷰프러스텀 을 정의하기 위한 파라미터
            //Fov, 종횡비, near, far
            camera.setPerspectiveProjection(glm::radians(120.f), aspect, 0.1f, 100.f);

            if (auto commandBuffer = lveRenderer.beginFrame()) {
                lveRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                lveRenderer.endSwapChainRenderPass(commandBuffer);
                lveRenderer.endFrame();
            }
        }
        vkDeviceWaitIdle(lveDevice.device());
    }

    std::unique_ptr<LveModel> createCubeModel(LveDevice& device, glm::vec3 offset) {
        std::vector<LveModel::Vertex> vertices{
        // left face (white)
        { { -.5f, -.5f, -.5f }, { .9f, .9f, .9f } },
        { {-.5f, .5f, .5f}, {.9f, .9f, .9f} },
        { {-.5f, -.5f, .5f}, {.9f, .9f, .9f} },
        { {-.5f, -.5f, -.5f}, {.9f, .9f, .9f} },
        { {-.5f, .5f, -.5f}, {.9f, .9f, .9f} },
        { {-.5f, .5f, .5f}, {.9f, .9f, .9f} },
            // right face (yellow)
        { {.5f, -.5f, -.5f}, {.8f, .8f, .1f} },
        { {.5f, .5f, .5f}, {.8f, .8f, .1f} },
        { {.5f, -.5f, .5f}, {.8f, .8f, .1f} },
        { {.5f, -.5f, -.5f}, {.8f, .8f, .1f} },
        { {.5f, .5f, -.5f}, {.8f, .8f, .1f} },
        { {.5f, .5f, .5f}, {.8f, .8f, .1f} },
            // top face (orange, remember y axis points down)
        { {-.5f, -.5f, -.5f}, {.9f, .6f, .1f} },
        { {.5f, -.5f, .5f}, {.9f, .6f, .1f} },
        { {-.5f, -.5f, .5f}, {.9f, .6f, .1f} },
        { {-.5f, -.5f, -.5f}, {.9f, .6f, .1f} },
        { {.5f, -.5f, -.5f}, {.9f, .6f, .1f} },
        { {.5f, -.5f, .5f}, {.9f, .6f, .1f} },
            // bottom face (red)
        { {-.5f, .5f, -.5f}, {.8f, .1f, .1f} },
        { {.5f, .5f, .5f}, {.8f, .1f, .1f} },
        { {-.5f, .5f, .5f}, {.8f, .1f, .1f} },
        { {-.5f, .5f, -.5f}, {.8f, .1f, .1f} },
        { {.5f, .5f, -.5f}, {.8f, .1f, .1f} },
        { {.5f, .5f, .5f}, {.8f, .1f, .1f} },
            // nose face (blue)
        { {-.5f, -.5f, 0.5f}, {.1f, .1f, .8f} },
        { {.5f, .5f, 0.5f}, {.1f, .1f, .8f} },
        { {-.5f, .5f, 0.5f}, {.1f, .1f, .8f} },
        { {-.5f, -.5f, 0.5f}, {.1f, .1f, .8f} },
        { {.5f, -.5f, 0.5f}, {.1f, .1f, .8f} },
        { {.5f, .5f, 0.5f}, {.1f, .1f, .8f} },
            // tail face (green)
        { {-.5f, -.5f, -0.5f}, {.1f, .8f, .1f} },
        { {.5f, .5f, -0.5f}, {.1f, .8f, .1f} },
        { {-.5f, .5f, -0.5f}, {.1f, .8f, .1f} },
        { {-.5f, -.5f, -0.5f}, {.1f, .8f, .1f} },
        { {.5f, -.5f, -0.5f}, {.1f, .8f, .1f} },
        { {.5f, .5f, -0.5f}, {.1f, .8f, .1f} },
    };
    for (auto& v : vertices) {
        v.position += offset;
    }
    return std::make_unique<LveModel>(device, vertices);
}

    //모델을 로드
    //현재는 삼각형 및 색상지정
    void FirstApp::loadgameObject() {
        std::shared_ptr<LveModel> lveModel = createCubeModel(lveDevice, { .0f, .0f, .0f });
        auto cube = LveGameObject::createGameObject();
        cube.model = lveModel;
        cube.transform.translation = { .0f, .0f, 2.5f };
        cube.transform.scale = { .5f, .5f, .5f };
        gameObjects.push_back(std::move(cube));
    }
}