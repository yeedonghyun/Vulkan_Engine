#pragma once

#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_swap_chain.hpp"
#include "lve_pipeline.hpp"
#include "lve_model.hpp"

#include <memory>
#include <vector>

namespace lve {
	class  FirstApp {
	public:
		//윈도우 크기
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		//지속적으로 실행
		void run();

	private:
		void loadgameObject();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void renderGameObject(VkCommandBuffer commandBuffer);

		//생성자를 사용해서 윈도우창 초기화
		LveWindow lveWindow{WIDTH, HEIGHT, "Hi Vulkan"};
		LveDevice lveDevice{ lveWindow };
		std::unique_ptr<LveSwapChain> lveSwapChain;

		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<LveGameObject> gameObject;
	};
}