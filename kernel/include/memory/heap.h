#ifndef HEAP_H
#define HEAP_H

#include "../lib.h"
#include "paging.h"

namespace memory::heap {
    struct HeapSegHdr{
        size_t length;
        HeapSegHdr* next;
        HeapSegHdr* last;
        bool free;
        auto combine_forward() -> void;
        auto combine_backward() -> void;
        auto split(size_t split_length) -> HeapSegHdr*;
    };

    auto init(void* heap_addr, size_t num_page) -> void;

    auto malloc(size_t size) -> void*;
    auto free(void* address) -> void;

    auto expand(size_t length) -> void;
}

struct nothrow_t {};
extern const nothrow_t nothrow;

auto operator new(size_t size) -> void *;
auto operator new(size_t size, const nothrow_t&) noexcept -> void *;
auto operator delete(void* ptr) noexcept -> void;
auto operator delete(void* ptr, const nothrow_t&) noexcept -> void;
auto operator delete(void* ptr, size_t size) noexcept -> void;

auto operator new[](size_t size) -> void *;
auto operator new[](size_t size, const nothrow_t&) noexcept -> void *;
auto operator delete[](void* ptr) noexcept -> void;
auto operator delete[](void* ptr, const nothrow_t&) noexcept -> void;
auto operator delete[](void* ptr, size_t size) noexcept -> void;

#endif