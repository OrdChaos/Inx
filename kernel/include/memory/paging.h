#ifndef PAGING_H
#define PAGING_H

#include "../lib.h"
#include "efimemory.h"
#include "memory.h"
#include "page_frame_allocator.h"

namespace memory {
    enum PTFlag {
        Present = 0,
        ReadWrite = 1,
        UserSuper = 2,
        WriteThrough = 3,
        CacheDisabled = 4,
        Accessed = 5,
        LargerPages = 7,
        Custom0 = 9,
        Custom1 = 10,
        Custom2 = 11,
        NX = 63 // IF SUPPORTED
    };

    struct PageDirectoryEntry {
        uint64_t value;
        auto set_flag(PTFlag flag, bool enabled) -> void;
        auto get_flag(PTFlag flag) -> bool;
        auto set_addr(uint64_t address) -> void;
        auto get_addr() -> uint64_t;
    };

    struct PageTable { 
        PageDirectoryEntry entries [512];
    }__attribute__((aligned(0x1000)));

    class PageMapIndexer {
        public:
            PageMapIndexer(uint64_t virtualAddress);
            uint64_t PDP_i;
            uint64_t PD_i;
            uint64_t PT_i;
            uint64_t P_i;
    };

    class PageTableManager {
        public:
            PageTableManager(PageTable* PML4_addr);
            PageTable* PML4;
            auto map_memory(void* virtual_memory, void* physical_memory) -> void;
    };

    auto get_global_page_table_manager() -> PageTableManager *;
}

#endif