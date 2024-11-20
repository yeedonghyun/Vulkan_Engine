#pragma once

#include "lve_device.hpp"
#include "lve_window.hpp"
#include "lve_swap_chain.hpp"
#include "lve_pipeline.hpp"

#include <memory>
#include <vector>

namespace lve {
	class  FirstApp {
	public:
		//������ ũ��
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		//���������� ����
		void run();

	private:
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		//�����ڸ� ����ؼ� ������â �ʱ�ȭ
		LveWindow lveWindow{WIDTH, HEIGHT, "Hi Vulkan"};
		LveDevice lveDevice{ lveWindow };
		LveSwapChain lveSwapChain{ lveDevice, lveWindow.getExtent()};

		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}