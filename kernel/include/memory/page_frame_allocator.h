#ifndef PAGE_FRAME_ALLOCATOR_H
#define PAGE_FRAME_ALLOCATOR_H

#include "../lib.h"
#include "efimemory.h"
#include "memory.h"

namespace memory {
    class PageFrameAllocator {
        auto init_bitmap(size_t bitmapSize, void* bufferAddress) -> void;
        auto reserve_page(void* address) -> void;
        auto reserve_pages(void* address, uint64_t num_page) -> void;
        auto unreserve_page(void* address) -> void;
        auto unreserve_pages(void* address, uint64_t num_page) -> void;

        public:
            Bitmap page_bitmap;
            PageFrameAllocator();
            PageFrameAllocator(MemoryMapInfo *memory_map);
            auto read_memory_map(MemoryMapInfo *memory_map) -> void;
            auto free_page(void* address) -> void;
            auto free_pages(void* address, uint64_t num_page) -> void;
            auto lock_page(void* address) -> void;
            auto lock_pages(void* address, uint64_t num_page) -> void;
            auto request_page() -> void*;
            auto get_free_RAM() -> uint64_t;
            auto get_used_RAM() -> uint64_t;
            auto get_reserved_RAM() -> uint64_t;
    };

    auto get_global_frame_alloc() -> PageFrameAllocator *;
}

#endif