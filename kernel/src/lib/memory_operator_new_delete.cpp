#include "../../include/memory/heap.h"

const nothrow_t nothrow = {};

auto operator new(size_t size) -> void* {
    if (size == 0) {
        size = 1;
    }
    void* ptr = memory::heap::malloc(size);
    if (!ptr) {
        while (true) {}
    }
    return ptr;
}

auto operator new(size_t size, const nothrow_t&) noexcept -> void* {
    return memory::heap::malloc(size);
}

auto operator delete(void* ptr) noexcept -> void {
    memory::heap::free(ptr);
}

auto operator delete(void* ptr, const nothrow_t&) noexcept -> void {
    memory::heap::free(ptr);
}

auto operator delete(void* ptr, size_t size) noexcept -> void {
    memory::heap::free(ptr);
}

auto operator new[](size_t size) -> void* {
    return operator new(size);
}

auto operator new[](size_t size, const nothrow_t&) noexcept -> void* {
    return operator new(size, nothrow);
}

auto operator delete[](void* ptr) noexcept -> void {
    operator delete(ptr);
}

auto operator delete[](void* ptr, const nothrow_t&) noexcept -> void {
    operator delete(ptr, nothrow);
}

auto operator delete[](void* ptr, size_t size) noexcept -> void {
    operator delete(ptr);
}