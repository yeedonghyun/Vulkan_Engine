#include "First_app.hpp"

namespace lve {

	//닫힘 이벤트 뜰때까지 반복해서 실행
	void lve::FirstApp::run()
	{
		while (!lveWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
}