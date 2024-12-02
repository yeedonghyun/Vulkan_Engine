#include "lve_buffer.hpp"

#include <cassert>
#include <cstring>

namespace lve {
    //사이즈 정렬
    //불칸은 특정 사이즈에 정렬되어야 성능이 최적화
    VkDeviceSize LveBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
        if (minOffsetAlignment > 0) {
            return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
        }
        return instanceSize;
    }

    LveBuffer::LveBuffer(
        LveDevice& device,
        VkDeviceSize instanceSize,
        uint32_t instanceCount,
        VkBufferUsageFlags usageFlags,
        VkMemoryPropertyFlags memoryPropertyFlags,
        VkDeviceSize minOffsetAlignment)
        : lveDevice{ device },
        instanceSize{ instanceSize },
        instanceCount{ instanceCount },
        usageFlags{ usageFlags },
        memoryPropertyFlags{ memoryPropertyFlags } {
        alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
        bufferSize = alignmentSize * instanceCount;
        device.createBuffer(bufferSize, usageFlags, memoryPropertyFlags, buffer, memory);
    }

    LveBuffer::~LveBuffer() {
        unmap();
        vkDestroyBuffer(lveDevice.device(), buffer, nullptr);
        vkFreeMemory(lveDevice.device(), memory, nullptr);
    }

    //cpu가 gpu메모리에 데이터를 작성할수 있도록함
    VkResult LveBuffer::map(VkDeviceSize size, VkDeviceSize offset) {
        assert(buffer && memory && "Called map on buffer before create");
        return vkMapMemory(lveDevice.device(), memory, offset, size, 0, &mapped);
    }

    //매핑해제
    void LveBuffer::unmap() {
        if (mapped) {
            vkUnmapMemory(lveDevice.device(), memory);
            mapped = nullptr;
        }
    }

    //매핑된 메모리에 데이터를 복사
    void LveBuffer::writeToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset) {
        assert(mapped && "Cannot copy to unmapped buffer");

        if (size == VK_WHOLE_SIZE) {
            memcpy(mapped, data, bufferSize);
        }
        else {
            char* memOffset = (char*)mapped;
            memOffset += offset;
            memcpy(memOffset, data, size);
        }
    }

    //cpu 가 작성한데이터를 gpu가 읽을수 있도록 메모리 동기화
    VkResult LveBuffer::flush(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkFlushMappedMemoryRanges(lveDevice.device(), 1, &mappedRange);
    }

    //gpu 가 작성한데이터를 cpu가 읽을수 있도록 메모리 동기화
    VkResult LveBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkInvalidateMappedMemoryRanges(lveDevice.device(), 1, &mappedRange);
    }

    VkDescriptorBufferInfo LveBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
        return VkDescriptorBufferInfo{
            buffer,
            offset,
            size,
        };
    }

    void LveBuffer::writeToIndex(void* data, int index) {
        writeToBuffer(data, instanceSize, index * alignmentSize);
    }

    VkResult LveBuffer::flushIndex(int index) { return flush(alignmentSize, index * alignmentSize); }

    VkDescriptorBufferInfo LveBuffer::descriptorInfoForIndex(int index) {
        return descriptorInfo(alignmentSize, index * alignmentSize);
    }

    VkResult LveBuffer::invalidateIndex(int index) {
        return invalidate(alignmentSize, index * alignmentSize);
    }
}  