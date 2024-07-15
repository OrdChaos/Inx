#include "../../include/memory/paging.h"

namespace memory {
    PageTableManager global_page_table_manager = NULL;

    auto get_global_page_table_manager() -> PageTableManager * {
        return &global_page_table_manager;
    }

    PageMapIndexer::PageMapIndexer(uint64_t virtualAddress) {
        virtualAddress >>= 12;
        P_i = virtualAddress & 0x1ff;
        virtualAddress >>= 9;
        PT_i = virtualAddress & 0x1ff;
        virtualAddress >>= 9;
        PD_i = virtualAddress & 0x1ff;
        virtualAddress >>= 9;
        PDP_i = virtualAddress & 0x1ff;
    }

    PageTableManager::PageTableManager(PageTable* PML4_addr) {
        this->PML4 = PML4_addr;
    }

    auto PageTableManager::map_memory(void* virtual_memory, void* physical_memory) -> void {
        PageMapIndexer indexer = PageMapIndexer((uint64_t)virtual_memory);
        PageDirectoryEntry PDE;
        PageFrameAllocator *_global_frame_alloc = get_global_frame_alloc();

        PDE = PML4->entries[indexer.PDP_i];
        PageTable* PDP;
        if (!PDE.get_flag(PTFlag::Present)) {
            PDP = (PageTable*)_global_frame_alloc->request_page();
            memset(PDP, 0, 0x1000);
            PDE.set_addr((uint64_t)PDP >> 12);
            PDE.set_flag(PTFlag::Present, true);
            PDE.set_flag(PTFlag::ReadWrite, true);
            PML4->entries[indexer.PDP_i] = PDE;
        }
        else {
            PDP = (PageTable*)((uint64_t)PDE.get_addr() << 12);
        }
        
        
        PDE = PDP->entries[indexer.PD_i];
        PageTable* PD;
        if (!PDE.get_flag(PTFlag::Present)) {
            PD = (PageTable*)_global_frame_alloc->request_page();
            memset(PD, 0, 0x1000);
            PDE.set_addr((uint64_t)PD >> 12);
            PDE.set_flag(PTFlag::Present, true);
            PDE.set_flag(PTFlag::ReadWrite, true);
            PDP->entries[indexer.PD_i] = PDE;
        }
        else {
            PD = (PageTable*)((uint64_t)PDE.get_addr() << 12);
        }

        PDE = PD->entries[indexer.PT_i];
        PageTable* PT;
        if (!PDE.get_flag(PTFlag::Present)) {
            PT = (PageTable*)_global_frame_alloc->request_page();
            memset(PT, 0, 0x1000);
            PDE.set_addr((uint64_t)PT >> 12);
            PDE.set_flag(PTFlag::Present, true);
            PDE.set_flag(PTFlag::ReadWrite, true);
            PD->entries[indexer.PT_i] = PDE;
        }
        else {
            PT = (PageTable*)((uint64_t)PDE.get_addr() << 12);
        }

        PDE = PT->entries[indexer.P_i];
        PDE.set_addr((uint64_t)physical_memory >> 12);
        PDE.set_flag(PTFlag::Present, true);
        PDE.set_flag(PTFlag::ReadWrite, true);
        PT->entries[indexer.P_i] = PDE;
    }

    auto PageDirectoryEntry::set_flag(PTFlag flag, bool enabled) -> void {
        uint64_t bitSelector = (uint64_t)1 << flag;
        value &= ~bitSelector;
        if (enabled) {
            value |= bitSelector;
        }
    }

    auto PageDirectoryEntry::get_flag(PTFlag flag) -> bool {
        uint64_t bitSelector = (uint64_t)1 << flag;
        return value & bitSelector > 0 ? true : false;
    }

    auto PageDirectoryEntry::get_addr() -> uint64_t {
        return (value & 0x000ffffffffff000) >> 12;
    }

    auto PageDirectoryEntry::set_addr(uint64_t address) -> void {
        address &= 0x000000ffffffffff;
        value &= 0xfff0000000000fff;
        value |= (address << 12);
    }
}