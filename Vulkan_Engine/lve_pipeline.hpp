#pragma once

#include <string>
#include <vector>

namespace lve {
	class LvePipeline {
	public:
		//shader ��� �޾Ƽ� shader ���� �а� �׽�Ʈ
		LvePipeline(const std::string &vertFilePath, const std::string& fragFilePath);

	private:
		//������ �а� �����ߴٸ� vector<char> ���� ������ ����
		//�����ߴٸ� throw 
		static std::vector<char> readFile(const std::string& filePath);

		//shader ���� �д� �Լ�(������ �а� �׽�Ʈ �ϴ� �κи� ������)
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
	};
}