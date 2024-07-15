#ifndef MEMORY_H
#define MEMORY_H

#include "../lib.h"
#include "efimemory.h"

namespace memory {
    auto get_memory_size(MemoryMapInfo *memory_map) -> uint64_t;

    class Bitmap{
        public:
            size_t size;
            uint8_t* buffer;
            auto operator[](uint64_t index) -> bool;
            auto set(uint64_t index, bool value) -> bool;
    };
}

#endif