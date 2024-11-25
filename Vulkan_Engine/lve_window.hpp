#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
namespace lve {
	class LveWindow {
	public:
		//윈도우 크기 이름 초기화
		LveWindow(int w, int h, std::string name);
		~LveWindow();

		//윈도우창은 한개만 생성될수 있기때문에 방지
		LveWindow(const LveWindow&) = delete;
		LveWindow &operator=(const LveWindow&) = delete;

		//창닫힘 확인함수
		bool shouldClose() { return glfwWindowShouldClose(window); };
		//윈도우 창의 크기 리턴
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
		bool wasWindowResized() { return framebufferResized; }
		void resetWindowResizeFlag() { framebufferResized = false; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		void initWindow();

		//윈도우 크기
		int width;
		int height;
		bool framebufferResized = false;

		//이름
		std::string windowName;
		GLFWwindow* window;
	};
}