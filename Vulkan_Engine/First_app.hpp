#pragma once

#include "lve_window.hpp"

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
	};
}