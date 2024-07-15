#include "../../include/memory/page_frame_allocator.h"

namespace memory {
    uint64_t free_memory;
    uint64_t reserved_memory;
    uint64_t used_memory;
    bool is_initialized = false;
    PageFrameAllocator global_frame_alloc;

    auto get_global_frame_alloc() -> PageFrameAllocator * {
        return &global_frame_alloc;
    }

    PageFrameAllocator::PageFrameAllocator() {
        free_memory = 0;
        reserved_memory = 0;
        used_memory = 0;
        is_initialized = false;
    }

    PageFrameAllocator::PageFrameAllocator(MemoryMapInfo *memory_map) {
        read_memory_map(memory_map);
    }

    auto PageFrameAllocator::read_memory_map(MemoryMapInfo *memory_map) -> void {
        if(is_initialized) return;
        is_initialized = true;
        void *max_free_mem_seg = NULL;
        size_t max_free_mem_seg_size = 0;

        uint64_t entries = memory_map->size / memory_map->desc_size;
        for (int i = 0; i < entries;i++) {
            MemoryMapDescriptor *desc = reinterpret_cast<MemoryMapDescriptor*>(
                reinterpret_cast<uint8_t*>(memory_map->memory_map) + (i * memory_map->desc_size)
            );
            if(desc->type == EfiReservedMemoryType) continue;
            if(desc->type == EfiConventionalMemory) {
                if(desc->pages_count * 4096 > max_free_mem_seg_size) {
                    max_free_mem_seg = desc->phys_addr;
                    max_free_mem_seg_size = desc->pages_count * 4096;
                }
            }
        }

        uint64_t memory_size = get_memory_size(memory_map);
        free_memory = memory_size;
        uint64_t bitmap_size = memory_size / 4096 / 8 + 1;

        init_bitmap(bitmap_size, max_free_mem_seg);
        lock_pages(&page_bitmap, page_bitmap.size / 4096 + 1);
        for (int i = 0; i < entries;i++) {
            MemoryMapDescriptor *desc = reinterpret_cast<MemoryMapDescriptor*>(
                reinterpret_cast<uint8_t*>(memory_map->memory_map) + (i * memory_map->desc_size)
            );
            if(desc->type == EfiReservedMemoryType) continue;
            if(desc->type != EfiConventionalMemory) {
                reserve_pages(desc->phys_addr, desc->pages_count);
            }
        }
    }

    auto PageFrameAllocator::init_bitmap(size_t bitmap_size, void* buffer_address) -> void {
        page_bitmap.size = bitmap_size;
        page_bitmap.buffer = (uint8_t*)buffer_address;
        for (size_t i = 0; i < bitmap_size; i++) {
            *(uint8_t*)(page_bitmap.buffer + i) = 0;
        }
    }

    uint64_t page_bitmap_index = 0;
    auto PageFrameAllocator::request_page() -> void* {
        for (; page_bitmap_index < page_bitmap.size * 8; page_bitmap_index++) {
            if (page_bitmap[page_bitmap_index]) continue;
            lock_page((void*)(page_bitmap_index * 4096));
            return (void*)(page_bitmap_index * 4096);
        }
        return nullptr;
    }

    auto PageFrameAllocator::free_page(void* address) -> void {
        uint64_t index = (uint64_t)address / 4096;
        if (!page_bitmap[index]) return;
        if(page_bitmap.set(index, false)) {
            free_memory += 4096;
            used_memory -= 4096;
            if(index < page_bitmap_index) page_bitmap_index = index;
        }
    }

    auto PageFrameAllocator::free_pages(void* address, uint64_t num_page) -> void {
        for (uint64_t t = 0; t < num_page; t++) {
            free_page((void*)((uint64_t)address + (t * 4096)));
        }
    }

    auto PageFrameAllocator::lock_page(void* address) -> void {
        uint64_t index = (uint64_t)address / 4096;
        if (page_bitmap[index]) return;
        if(page_bitmap.set(index, true)) {
            free_memory -= 4096;
            used_memory += 4096;
        }
    }

    auto PageFrameAllocator::lock_pages(void* address, uint64_t num_page) -> void {
        for (uint64_t t = 0; t < num_page; t++) {
            lock_page((void*)((uint64_t)address + (t * 4096)));
        }
    }

    auto PageFrameAllocator::unreserve_page(void* address) -> void {
        uint64_t index = (uint64_t)address / 4096;
        if (!page_bitmap[index]) return;
        if(page_bitmap.set(index, false)) {
            free_memory += 4096;
            reserved_memory -= 4096;
            if(index < page_bitmap_index) page_bitmap_index = index;
        }
    }

    auto PageFrameAllocator::unreserve_pages(void* address, uint64_t num_page) -> void {
        for (uint64_t t = 0; t < num_page; t++) {
            unreserve_page((void*)((uint64_t)address + (t * 4096)));
        }
    }

    auto PageFrameAllocator::reserve_page(void* address) -> void {
        uint64_t index = (uint64_t)address / 4096;
        if (page_bitmap[index]) return;
        if(page_bitmap.set(index, true)) {
            free_memory -= 4096;
            reserved_memory += 4096;
        }
    }

    auto PageFrameAllocator::reserve_pages(void* address, uint64_t num_page) -> void {
        for (uint64_t t = 0; t < num_page; t++) {
            reserve_page((void*)((uint64_t)address + (t * 4096)));
        }
    }

    auto PageFrameAllocator::get_free_RAM() -> uint64_t {
        return free_memory;
    }

    auto PageFrameAllocator::get_used_RAM() -> uint64_t {
        return used_memory;
    }

    auto PageFrameAllocator::get_reserved_RAM() -> uint64_t {
        return reserved_memory;
    }
}