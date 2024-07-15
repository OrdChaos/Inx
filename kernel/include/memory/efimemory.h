#ifndef EFI_MEMORY_H
#define EFI_MEMORY_H

#include "../lib.h"

namespace memory {
    enum MemoryTypes : uint32_t {
        EfiReservedMemoryType,
        EfiLoaderCode,
        EfiLoaderData,
        EfiBootServicesCode,
        EfiBootServicesData,
        EfiRuntimeServicesCode,
        EfiRuntimeServicesData,
        EfiConventionalMemory,
        EfiUnusableMemory,
        EfiACPIReclaimMemory,
        EfiACPIMemoryNVS,
        EfiMemoryMappedIO,
        EfiMemoryMappedIOPortSpace,
        EfiPalCode
    };

    struct MemoryMapDescriptor {
        MemoryTypes type;
        void *phys_addr;
        void *virt_addr;
        uint64_t pages_count;
        uint64_t attributes;
    };

    struct MemoryMapInfo {
        MemoryMapDescriptor *memory_map;
        uint64_t size;
        uint64_t desc_size;
    };
}

#endif