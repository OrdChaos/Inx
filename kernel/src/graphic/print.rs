use super::{FrameBufferInfo, psf1::{Psf1Font, FONT_WIDTH}};

pub fn print_text(framebuffer: &FrameBufferInfo, x: u32, y: u32, text: &str, color: u32) {
    let font_bytes = include_bytes!("CyrAsia-Terminus16.psf");

    let font = Psf1Font::from_bytes(font_bytes).expect("Failed to load PSF1 font");

    let mut x_offset = x;
    for ch in text.chars() {
        font.draw_char(framebuffer, x_offset, y, ch, color);
        x_offset += FONT_WIDTH;
    }
}