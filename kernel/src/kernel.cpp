#include "../include/kernel_utils.h"

extern"C" __attribute__((sysv_abi)) void kernel_main(utils::BootInfo *boot_info) {
    utils::KernelInfo kernel_info = utils::init(boot_info);
    graphic::draw::whole_screen(boot_info->frame_buffer, 0xff000000);
    graphic::print::string(boot_info->frame_buffer, 0, 0, 0xffffffff, "Hello, Inx!");
    char buffer[256];
    sprintf(
        buffer,
        "MEMORY: Free = %d KB; Used = %d KB; Reserved = %d KB; MALLOCTEST = %llx",
        kernel_info.global_frame_alloc->get_free_RAM() / 1024,
        kernel_info.global_frame_alloc->get_used_RAM() / 1024,
        kernel_info.global_frame_alloc->get_reserved_RAM() / 1024,
        (uint64_t)memory::heap::malloc(0x100)
    );
    graphic::print::string(boot_info->frame_buffer, 0, 16, 0xffffffff, buffer);

    while(true) {

    }

    return;
}