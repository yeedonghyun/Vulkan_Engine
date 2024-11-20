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

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		void initWindow();

		//윈도우 크기
		const int width;
		const int height;

		//이름
		std::string windowName;
		GLFWwindow* window;
	};
}