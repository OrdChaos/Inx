pub mod print;
pub mod psf1;

#[repr(C)]
#[derive(PartialEq, Eq, PartialOrd, Ord, Debug, Clone, Copy)]
pub struct FrameBufferInfo {
    pub base_addr: u64,
    pub screen_width: u32,
    pub screen_height: u32,
    pub stride: u32,
}

pub fn draw_point(framebuffer: &FrameBufferInfo, x: u32, y: u32, color: u32) {
    unsafe {
        if x < framebuffer.screen_width && y < framebuffer.screen_height {
            let framebuffer_ptr = framebuffer.base_addr as *mut u8;
            let pixel_ptr = framebuffer_ptr.offset((y * framebuffer.stride * 4 + x * 4) as isize);

            let a = ((color >> 24) & 0xFF) as u8;
            let r = ((color >> 16) & 0xFF) as u8;
            let g = ((color >> 8) & 0xFF) as u8;
            let b = (color & 0xFF) as u8;

            *pixel_ptr.offset(0) = b;
            *pixel_ptr.offset(1) = g;
            *pixel_ptr.offset(2) = r;
            *pixel_ptr.offset(3) = a;
        }
    }
}

pub fn draw_box(framebuffer: &FrameBufferInfo, x0: u32, y0: u32, x1: u32, y1: u32, color: u32) {
    for y_offset in y0..y1 {
        for x_offset in x0..x1 {
            draw_point(framebuffer, x_offset, y_offset, color);
        }
    }
}

pub fn fill_screen(framebuffer: &FrameBufferInfo, color: u32) {
    draw_box(framebuffer, 0, 0, framebuffer.screen_width, framebuffer.screen_height, color);
}