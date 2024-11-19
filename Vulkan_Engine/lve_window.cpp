#include "lve_window.hpp"

namespace lve {

	//멤버변수 초기화
	LveWindow::LveWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name } {
		initWindow();
	}

	//윈도우 종료시 윈도우 닫힘 and 메모리 삭제
	LveWindow::~LveWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void LveWindow::initWindow() {
		glfwInit();
		//기본값은 OpenGL 이지만 GLFW_NO_API로하면 Vulkan 까지 가능
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//GLFW_FALSE 초반이라 창크기 변경 막아둠
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}
}