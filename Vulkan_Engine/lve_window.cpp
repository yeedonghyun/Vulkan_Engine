#include "lve_window.hpp"

namespace lve {

	//������� �ʱ�ȭ
	LveWindow::LveWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name } {
		initWindow();
	}

	//������ ����� ������ ���� and �޸� ����
	LveWindow::~LveWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void LveWindow::initWindow() {
		glfwInit();
		//�⺻���� OpenGL ������ GLFW_NO_API���ϸ� Vulkan ���� ����
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//GLFW_FALSE �ʹ��̶� âũ�� ���� ���Ƶ�
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}
}