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
		//������ â�� ũ�� ����
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
		bool wasWindowResized() { return framebufferResized; }
		void resetWindowResizeFlag() { framebufferResized = false; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		void initWindow();

		//������ ũ��
		int width;
		int height;
		bool framebufferResized = false;

		//�̸�
		std::string windowName;
		GLFWwindow* window;
	};
}