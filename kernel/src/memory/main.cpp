#include "../../include/memory/memory.h"

namespace memory {
    auto get_memory_size(MemoryMapInfo *memory_map) -> uint64_t {
        uint64_t memory_size_bytes = 0;

        uint64_t entries = memory_map->size / memory_map->desc_size;
        for (int i = 0; i < entries;i++) {
            MemoryMapDescriptor *desc = reinterpret_cast<MemoryMapDescriptor*>(
                reinterpret_cast<uint8_t*>(memory_map->memory_map) + (i * memory_map->desc_size)
            );
            if(desc->type == EfiReservedMemoryType) continue;
            memory_size_bytes += desc->pages_count * 4096;
        }

        return memory_size_bytes;
    }

    auto Bitmap::operator[](uint64_t index) -> bool {
        if(index > size * 8) return false;
        uint64_t byteIndex = index / 8;
        uint8_t bitIndex = index % 8;
        uint8_t bitIndexer = 0x80 >> bitIndex;
        if ((buffer[byteIndex] & bitIndexer) > 0){
            return true;
        }
        return false;
    }

    auto Bitmap::set(uint64_t index, bool value) -> bool {
        if(index > size * 8) return false;
        uint64_t byteIndex = index / 8;
        uint8_t bitIndex = index % 8;
        uint8_t bitIndexer = 0x80 >> bitIndex;
        buffer[byteIndex] &= ~bitIndexer;
        if (value){
            buffer[byteIndex] |= bitIndexer;
        }
        return true;
    }
}

