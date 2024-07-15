#include "../include/kernel_utils.h"

namespace utils {
    KernelInfo kernel_info;

    auto init_memory(const graphic::FrameBufferInfo *frame_buffer, memory::MemoryMapInfo *memory_map) -> void {
        memory::PageFrameAllocator *_global_frame_alloc = memory::get_global_frame_alloc();
        memory::PageTableManager *_page_table_manager = memory::get_global_page_table_manager();

        *_global_frame_alloc = memory::PageFrameAllocator(memory_map);
        uint64_t kernel_size = (uint64_t)&_kernel_end - (uint64_t)&_kernel_begin;
        uint64_t kernel_pages = kernel_size / 0x1000 + 1;
        _global_frame_alloc->lock_pages(&_kernel_begin, kernel_pages);

        memory::PageTable *PML4 = (memory::PageTable *)_global_frame_alloc->request_page();
        memset(PML4, 0, 0x1000);
        *_page_table_manager = memory::PageTableManager(PML4);

        //From 0, not _kernel_begin(1MB)
        for (uint64_t i = 0; i < memory::get_memory_size(memory_map);i += 0x1000) {
            _page_table_manager->map_memory((void *)i, (void *)i);
        }

        uint64_t frame_buffer_base = (uint64_t)frame_buffer->base_addr;
        uint64_t frame_buffer_size = (uint64_t)frame_buffer->screen_width * (uint64_t)frame_buffer->screen_height * 4;
        _global_frame_alloc->lock_pages((void *)frame_buffer_base, frame_buffer_size / 4096 + 1);
        for (uint64_t i = frame_buffer_base; i < frame_buffer_base + frame_buffer_size;i += 0x1000) {
            _page_table_manager->map_memory((void *)i, (void *)i);
        }

        asm("mov %0, %%cr3" : : "r"(PML4));

        kernel_info.page_table_manager = _page_table_manager;
        kernel_info.global_frame_alloc = _global_frame_alloc;
    }

    auto init(BootInfo *boot_info) -> KernelInfo {
        init_memory(boot_info->frame_buffer, boot_info->memory_map);
        memory::heap::init((void *)0x0000100000000000, 0x10);
        graphic::init();
        return kernel_info;
    }
}