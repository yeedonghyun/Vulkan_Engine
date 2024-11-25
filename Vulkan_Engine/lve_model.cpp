#include "lve_model.hpp"

namespace lve {

	LveModel::LveModel(LveDevice &device, const std::vector<Vertex> &vertices) : lveDevice{device} {
		//vertex �����͸� gpu�޸𸮷� ���ε�
		createVertexBuffers(vertices);
	}

	LveModel::~LveModel() {
		//���ۿ� �޸� ����
		vkDestroyBuffer(lveDevice.device(), vertexBuffer, nullptr);
		vkFreeMemory(lveDevice.device(), vertexBufferMemory, nullptr);
	}

	//���� ���� �� �޸� �Ҵ�
	void LveModel::createVertexBuffers(const std::vector<Vertex>& verticse)
	{
		//vertex ����
		vertexCount = static_cast<uint32_t>(verticse.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(verticse[0]) * vertexCount;
		
		//���� ���� ����
		lveDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBuffer,
			vertexBufferMemory);

		void* data;
		//Cpu �� Gpu ����
		vkMapMemory(lveDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
		//���� ī��
		memcpy(data, verticse.data(), static_cast<size_t>(bufferSize));
		//���� ����
		vkUnmapMemory(lveDevice.device(), vertexBufferMemory);
	}

	//���� ���۸� ��ɹ��ۿ� ���ε� (�����Ҽ� ������)
	void LveModel::bind(VkCommandBuffer CommandBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(CommandBuffer, 0, 1, buffers, offsets);
	}

	//���ε��� ���� ���ۿ� ���� �׸��� ����� ����
	void LveModel::draw(VkCommandBuffer CommandBuffer)
	{
		vkCmdDraw(CommandBuffer, vertexCount, 1, 0, 0);
	}

	//���ε� ���
	std::vector<VkVertexInputBindingDescription> LveModel::Vertex::getBindingDescriptions() {
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	//������������ ���̾ƿ�
	std::vector<VkVertexInputAttributeDescription> LveModel::Vertex::getAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = 0;
		return attributeDescriptions;
	}
}
