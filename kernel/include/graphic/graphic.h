#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "../lib.h"

namespace graphic {
    struct FrameBufferInfo {
        uint64_t base_addr;
        uint32_t screen_width;
        uint32_t screen_height;
        uint32_t stride;
    };

    namespace draw {
        auto point(const FrameBufferInfo *frame_buffer, uint32_t x, uint32_t y, uint32_t color) -> void;
        auto box(const FrameBufferInfo *frame_buffer, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t border_thickness, uint32_t color) -> void;
        auto box_filled(const FrameBufferInfo *frame_buffer, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color) -> void;
        auto whole_screen(const FrameBufferInfo *frame_buffer, uint32_t color) -> void;
        auto circle(const FrameBufferInfo *frame_buffer, uint32_t x, uint32_t y, uint32_t radius, uint32_t border_thickness, uint32_t color) -> void;
        auto circle_filled(const FrameBufferInfo *frame_buffer, uint32_t x, uint32_t y, uint32_t radius, uint32_t color) -> void;
        auto line(const FrameBufferInfo *frame_buffer, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t thickness, uint32_t color) -> void;
    }

    namespace print {
        namespace psf1 {
            auto loadFont(const uint8_t *font_bytes, size_t size) -> bool;
        }
        auto character(const FrameBufferInfo *frame_buffer, uint32_t x, uint32_t y, uint32_t color, const unsigned char c) -> void;
        auto string(const FrameBufferInfo *frame_buffer, uint32_t x, uint32_t y, uint32_t color, const char *s) -> void;
    }

    auto init() -> bool;
}

#endif