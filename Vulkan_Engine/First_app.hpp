#pragma once

#include "lve_device.hpp"
#include "lve_window.hpp"
#include "lve_pipeline.hpp"

namespace lve {
	class  FirstApp {
	public:
		//������ ũ��
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		//���������� ����
		void run();

	private:
		//�����ڸ� ����ؼ� ������â �ʱ�ȭ
		LveWindow lveWindow{WIDTH, HEIGHT, "Hi Vulkan"};
		//
		LveDevice LveDevice{ lveWindow };
		//shader ��� *�����ϵ�*
		LvePipeline lvePipeline{ LveDevice, "Shaders/simple_shader.vert.spv", "Shaders/simple_shader.frag.spv" , 
			LvePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};

	};
}