#include "../../include/memory/heap.h"

namespace memory::heap {
    void* heapStart;
    void* heapEnd;
    HeapSegHdr* LastHdr;

    auto init(void* heap_addr, size_t num_page) -> void {
        PageFrameAllocator *_global_frame_alloc = get_global_frame_alloc();
        PageTableManager *_page_table_manager = get_global_page_table_manager();
        void* pos = heap_addr;

        for (size_t i = 0; i < num_page; i++){
            _page_table_manager->map_memory(pos, _global_frame_alloc->request_page());
            pos = (void*)((size_t)pos + 0x1000);
        }

        size_t heapLength = num_page * 0x1000;

        heapStart = heap_addr;
        heapEnd = (void*)((size_t)heapStart + heapLength);
        HeapSegHdr* startSeg = (HeapSegHdr*)heap_addr;
        startSeg->length = heapLength - sizeof(HeapSegHdr);
        startSeg->next = NULL;
        startSeg->last = NULL;
        startSeg->free = true;
        LastHdr = startSeg;
    }

    auto free(void* address) -> void {
        HeapSegHdr* segment = (HeapSegHdr*)address - 1;
        segment->free = true;
        segment->combine_forward();
        segment->combine_backward();
    }

    auto malloc(size_t size) -> void* {
        if (size % 0x10 > 0){ // it is not a multiple of 0x10
            size -= (size % 0x10);
            size += 0x10;
        }

        if (size == 0) return NULL;

        HeapSegHdr* currentSeg = (HeapSegHdr*) heapStart;
        while(true){
            if(currentSeg->free){
                if (currentSeg->length > size){
                    currentSeg->split(size);
                    currentSeg->free = false;
                    return (void*)((uint64_t)currentSeg + sizeof(HeapSegHdr));
                }
                if (currentSeg->length == size){
                    currentSeg->free = false;
                    return (void*)((uint64_t)currentSeg + sizeof(HeapSegHdr));
                }
            }
            if (currentSeg->next == NULL) break;
            currentSeg = currentSeg->next;
        }
        expand(size);
        return malloc(size);
    }

    auto HeapSegHdr::split(size_t split_length) -> HeapSegHdr* {
        if (split_length < 0x10) return NULL;
        int64_t splitSegLength = length - split_length - (sizeof(HeapSegHdr));
        if (splitSegLength < 0x10) return NULL;

        HeapSegHdr* newSplitHdr = (HeapSegHdr*) ((size_t)this + split_length + sizeof(HeapSegHdr));
        next->last = newSplitHdr;
        newSplitHdr->next = next;
        next = newSplitHdr;
        newSplitHdr->last = this;
        newSplitHdr->length = splitSegLength;
        newSplitHdr->free = free;
        length = split_length;

        if (LastHdr == this) LastHdr = newSplitHdr;
        return newSplitHdr;
    }

    auto expand(size_t length) -> void {
        PageFrameAllocator *_global_frame_alloc = get_global_frame_alloc();
        PageTableManager *_page_table_manager = get_global_page_table_manager();

        if (length % 0x1000) {
            length -= length % 0x1000;
            length += 0x1000;
        }

        size_t num_page = length / 0x1000;
        HeapSegHdr* newSegment = (HeapSegHdr*)heapEnd;

        for (size_t i = 0; i < num_page; i++){
            _page_table_manager->map_memory(heapEnd, _global_frame_alloc->request_page());
            heapEnd = (void*)((size_t)heapEnd + 0x1000);
        }

        newSegment->free = true;
        newSegment->last = LastHdr;
        LastHdr->next = newSegment;
        LastHdr = newSegment;
        newSegment->next = NULL;
        newSegment->length = length - sizeof(HeapSegHdr);
        newSegment->combine_backward();

    }

    auto HeapSegHdr::combine_forward() -> void {
        if (next == NULL) return;
        if (!next->free) return;

        if (next == LastHdr) LastHdr = this;

        if (next->next != NULL){
            next->next->last = this;
        }

        length = length + next->length + sizeof(HeapSegHdr);
    }

    auto HeapSegHdr::combine_backward() -> void {
        if (last != NULL && last->free) last->combine_forward();
    }
}