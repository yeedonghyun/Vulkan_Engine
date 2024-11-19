#pragma once

#include "lve_window.hpp"
#include "lve_pipeline.hpp"

namespace lve {
	class  FirstApp {
	public:
		//윈도우 크기
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		//지속적으로 실행
		void run();

	private:
		//생성자를 사용해서 윈도우창 초기화
		LveWindow lveWindow{WIDTH, HEIGHT, "Hi Vulkan"};
		//shader 경로 *컴파일된*
		LvePipeline lvePipeline{ "Shaders/simple_shader.vert.spv", "Shaders/simple_shader.frag.spv" };

	};
}