#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include "../include/include.h"

extern uint64_t _kernel_begin;
extern uint64_t _kernel_end;

namespace utils {
    struct BootInfo {
        const graphic::FrameBufferInfo *frame_buffer;
        memory::MemoryMapInfo *memory_map;
    };

    struct KernelInfo {
        memory::PageTableManager *page_table_manager;
        memory::PageFrameAllocator *global_frame_alloc;
    };

    auto init_memory(const graphic::FrameBufferInfo *frame_buffer, memory::MemoryMapInfo *memory_map) -> void;
    auto init(BootInfo *boot_info) -> KernelInfo;
}

#endif