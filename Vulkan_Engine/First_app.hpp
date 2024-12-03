#pragma once

#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_window.hpp"
#include "lve_renderer.hpp"
#include "lve_descriptors.hpp"

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
		void loadGameObject();

		//생성자를 사용해서 윈도우창 초기화
		LveWindow lveWindow{WIDTH, HEIGHT, "Hi Vulkan"};
		LveDevice lveDevice{ lveWindow };
		LveRenderer lveRenderer{ lveWindow, lveDevice };

		std::unique_ptr<LveDescriptorPool> globalPool{};
		std::vector<LveGameObject> gameObjects;
	};
}