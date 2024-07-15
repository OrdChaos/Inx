#include "../../include/graphic/graphic.h"

namespace graphic::print {
    namespace psf1 {
        struct PSF1Header {
            uint8_t magic[2];
            uint8_t mode;
            uint8_t charsize;
        };

        PSF1Header fontHeader;
        const uint8_t* fontData = nullptr;
        uint32_t numChars = 0;
        uint32_t FONT_WIDTH = 0;

        auto loadFont(const uint8_t* font_bytes, size_t size) -> bool {
            if (size < sizeof(PSF1Header)) {
                return false;
            }

            memcpy(&fontHeader, font_bytes, sizeof(PSF1Header));

            if (fontHeader.magic[0] != 0x36 || fontHeader.magic[1] != 0x04) {
                return false;
            }

            numChars = (size - sizeof(PSF1Header)) / fontHeader.charsize;

            fontData = font_bytes + sizeof(PSF1Header);
            FONT_WIDTH = 8; // 8*16 FONTS

            return true;
        }
    }

    auto character(const FrameBufferInfo* frame_buffer, uint32_t x, uint32_t y, uint32_t color, const unsigned char c) -> void {
        if (c >= psf1::numChars) {
            return;
        }
        const uint8_t* glyph = &psf1::fontData[c * psf1::fontHeader.charsize];
        for (uint8_t dy = 0; dy < 16; ++dy) {
            for (uint8_t dx = 0; dx < 8; ++dx) {
                if (glyph[dy] & (0x80 >> dx)) {
                    graphic::draw::point(frame_buffer, x + dx, y + dy, color);
                }
            }
        }
    }

    auto string(const FrameBufferInfo* frame_buffer, uint32_t x, uint32_t y, uint32_t color, const char* s) -> void {
        size_t offset = 0;
        for (const char* ptr = s; *ptr != '\0'; ptr++) {
            unsigned char c = *ptr;
            character(frame_buffer, x + psf1::FONT_WIDTH * offset, y, color, c);
            offset++;
        }
    }
}