#include "First_app.hpp"

namespace lve {

	//���� �̺�Ʈ �㶧���� �ݺ��ؼ� ����
	void lve::FirstApp::run()
	{
		while (!lveWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
}