#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
namespace lve {
	class LveWindow {
	public:
		//������ ũ�� �̸� �ʱ�ȭ
		LveWindow(int w, int h, std::string name);
		~LveWindow();

		//������â�� �Ѱ��� �����ɼ� �ֱ⶧���� ����
		LveWindow(const LveWindow&) = delete;
		LveWindow &operator=(const LveWindow&) = delete;

		//â���� Ȯ���Լ�
		bool shouldClose() { return glfwWindowShouldClose(window); };

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		void initWindow();

		//������ ũ��
		const int width;
		const int height;

		//�̸�
		std::string windowName;
		GLFWwindow* window;
	};
}