#![no_std]
#![no_main]

mod graphic;

use core::panic::PanicInfo;
use graphic::{fill_screen, FrameBufferInfo};
use graphic::print::print_text;

#[no_mangle]
pub extern "sysv64" fn _start(framebuffer: &FrameBufferInfo) {
    fill_screen(framebuffer, 0xffffffff);

    let mut buf = [0u8; 64];
    let s = format_no_std::show(
        &mut buf,
        format_args!("Test String {}: {}", "foo", 42),
    ).unwrap();

    print_text(framebuffer, 0, 0, s, 0xff000000);

    print_text(framebuffer, 0, 100, "Hello, World!", 0xff000000);

    loop{}
}

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    loop {}
}