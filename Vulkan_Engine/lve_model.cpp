#include "lve_model.hpp"

namespace lve {
	LveModel::LveModel(LveDevice& device, const LveModel::Builder& builder) : lveDevice{ device } {
		//vertex 데이터를 gpu메모리로 업로드
		createVertexBuffers(builder.vertices);
		createIndexBuffers(builder.indices);
	}

	LveModel::~LveModel() {
		//버퍼와 메모리 해제
		vkDestroyBuffer(lveDevice.device(), vertexBuffer, nullptr);
		vkFreeMemory(lveDevice.device(), vertexBufferMemory, nullptr);

		if (hasIndexBuffer) {
			vkDestroyBuffer(lveDevice.device(), indexBuffer, nullptr);
			vkFreeMemory(lveDevice.device(), indexBufferMemory, nullptr);
		}
	}

	//버퍼 생성 및 메모리 할당
	void LveModel::createVertexBuffers(const std::vector<Vertex>& verticse)
	{
		//vertex 개수
		vertexCount = static_cast<uint32_t>(verticse.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(verticse[0]) * vertexCount;
		
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		//정점 버퍼 생성
		lveDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory);

		void* data;
		//Cpu 와 Gpu 연결
		vkMapMemory(lveDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		//버퍼 카피
		memcpy(data, verticse.data(), static_cast<size_t>(bufferSize));
		//연결 해제
		vkUnmapMemory(lveDevice.device(), stagingBufferMemory);

		lveDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			vertexBuffer,
			vertexBufferMemory);

		lveDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
		vkDestroyBuffer(lveDevice.device(), stagingBuffer, nullptr);
		vkFreeMemory(lveDevice.device(), stagingBufferMemory, nullptr);
	}

	void LveModel::createIndexBuffers(const std::vector<uint32_t>& indices)
	{
		indexCount = static_cast<uint32_t>(indices.size());
		hasIndexBuffer = indexCount > 0;

		if (!hasIndexBuffer) {
			return;
		}

		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		//정점 버퍼 생성
		lveDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory);

		void* data;
		//Cpu 와 Gpu 연결
		vkMapMemory(lveDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		//버퍼 카피
		memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
		//연결 해제
		vkUnmapMemory(lveDevice.device(), stagingBufferMemory);

		lveDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			indexBuffer,
			indexBufferMemory);

		lveDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);
		vkDestroyBuffer(lveDevice.device(), stagingBuffer, nullptr);
		vkFreeMemory(lveDevice.device(), stagingBufferMemory, nullptr);
	}

	//정점 버퍼를 명령버퍼에 바인딩 (변경할수 없도록)
	void LveModel::bind(VkCommandBuffer CommandBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(CommandBuffer, 0, 1, buffers, offsets);

		if (hasIndexBuffer) {
			vkCmdBindIndexBuffer(CommandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		}
	}

	//바인딩된 정점 버퍼에 대해 그리기 명령을 내림
	void LveModel::draw(VkCommandBuffer CommandBuffer)
	{
		if (hasIndexBuffer) {
			vkCmdDrawIndexed(CommandBuffer, indexCount, 1, 0, 0, 0);
		}
		else {
			vkCmdDraw(CommandBuffer, vertexCount, 1, 0, 0);
		}
	}

	//바인딩 방식
	std::vector<VkVertexInputBindingDescription> LveModel::Vertex::getBindingDescriptions() {
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	//정점데이터의 레이아웃
	std::vector<VkVertexInputAttributeDescription> LveModel::Vertex::getAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = 0;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);
		return attributeDescriptions;
	}
}
