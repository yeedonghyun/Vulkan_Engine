#pragma once

#include <string>
#include <vector>

namespace lve {
	class LvePipeline {
	public:
		//shader 경로 받아서 shader 파일 읽고 테스트
		LvePipeline(const std::string &vertFilePath, const std::string& fragFilePath);

	private:
		//파일을 읽고 성공했다면 vector<char> 파일 정보를 리턴
		//실패했다면 throw 
		static std::vector<char> readFile(const std::string& filePath);

		//shader 파일 읽는 함수(파일을 읽고 테스트 하는 부분만 구현됨)
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
	};
}