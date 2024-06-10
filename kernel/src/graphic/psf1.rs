#[repr(C)]
#[derive(Debug)]
pub struct Psf1Header {
    pub magic: [u8; 2],
    pub mode: u8,
    pub charsize: u8,
}

pub struct Psf1Font<'a> {
    pub header: &'a Psf1Header,
    pub glyphs: &'a [u8],
}

pub const FONT_WIDTH: u32 = 8;

impl<'a> Psf1Font<'a> {
    pub fn from_bytes(bytes: &'a [u8]) -> Result<Self, &'static str> {
        if bytes.len() < core::mem::size_of::<Psf1Header>() {
            return Err("Invalid PSF1 font file: file too small");
        }

        let header = unsafe { &*(bytes.as_ptr() as *const Psf1Header) };

        // Check the magic number
        if header.magic != [0x36, 0x04] {
            return Err("Invalid PSF1 magic number");
        }

        let glyphs = &bytes[core::mem::size_of::<Psf1Header>()..];

        Ok(Self {
            header,
            glyphs,
        })
    }

    pub fn draw_char(&self, framebuffer: &super::FrameBufferInfo, x: u32, y: u32, ch: char, color: u32) {
        let glyph_index = ch as usize;
        let charsize = self.header.charsize as usize;
        let glyph_start = glyph_index * charsize;
        let glyph = &self.glyphs[glyph_start..glyph_start + charsize];

        for (row, &row_data) in glyph.iter().enumerate() {
            for col in 0..8 {
                if (row_data & (0b10000000 >> col)) != 0 {
                    super::draw_point(framebuffer, x + col as u32, y + row as u32, color);
                }
            }
        }
    }
}