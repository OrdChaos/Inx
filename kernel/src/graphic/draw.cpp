#include "../../include/graphic/graphic.h"

namespace graphic::draw {
    auto point(const FrameBufferInfo* frame_buffer, uint32_t x, uint32_t y, uint32_t color) -> void {
        if (x >= frame_buffer->screen_width || y >= frame_buffer->screen_height) {
            return;
        }

        uint32_t* pixel = reinterpret_cast<uint32_t*>(
            frame_buffer->base_addr + (y * frame_buffer->stride + x) * sizeof(uint32_t)
        );

        *pixel = color;
    }

    auto box(const FrameBufferInfo* frame_buffer, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t border_thickness, uint32_t color) -> void {
        for (uint32_t y = y0; y < y0 + border_thickness && y <= y1; ++y) {
            for (uint32_t x = x0; x <= x1; ++x) {
                point(frame_buffer, x, y, color);
            }
        }

        for (uint32_t y = y1 - border_thickness + 1; y <= y1 && y >= y0; ++y) {
            for (uint32_t x = x0; x <= x1; ++x) {
                point(frame_buffer, x, y, color);
            }
        }

        for (uint32_t x = x0; x < x0 + border_thickness && x <= x1; ++x) {
            for (uint32_t y = y0; y <= y1; ++y) {
                point(frame_buffer, x, y, color);
            }
        }

        for (uint32_t x = x1 - border_thickness + 1; x <= x1 && x >= x0; ++x) {
            for (uint32_t y = y0; y <= y1; ++y) {
                point(frame_buffer, x, y, color);
            }
        }
    }

    auto box_filled(const FrameBufferInfo* frame_buffer, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color) -> void {
        for (uint32_t y = y0; y <= y1; y++) {
            for (uint32_t x = x0; x <= x1; x++) {
                point(frame_buffer, x, y, color);
            }
        }
    }

    auto whole_screen(const FrameBufferInfo* frame_buffer, uint32_t color) -> void {
        box_filled(frame_buffer, 0, 0, frame_buffer->screen_width - 1, frame_buffer->screen_height - 1, color);
    }

    auto circle(const FrameBufferInfo* frame_buffer, uint32_t x_center, uint32_t y_center, uint32_t radius, uint32_t border_thickness, uint32_t color) -> void {
        int x = radius;
        int y = 0;
        int err = 0;

        while (x >= y) {
            for (int i = -border_thickness / 2; i <= border_thickness / 2; ++i) {
                point(frame_buffer, x_center + x + i, y_center + y, color);
                point(frame_buffer, x_center + y + i, y_center + x, color);
                point(frame_buffer, x_center - y + i, y_center + x, color);
                point(frame_buffer, x_center - x + i, y_center + y, color);
                point(frame_buffer, x_center - x + i, y_center - y, color);
                point(frame_buffer, x_center - y + i, y_center - x, color);
                point(frame_buffer, x_center + y + i, y_center - x, color);
                point(frame_buffer, x_center + x + i, y_center - y, color);
            }

            if (err <= 0) {
                y += 1;
                err += 2 * y + 1;
            }
            if (err > 0) {
                x -= 1;
                err -= 2 * x + 1;
            }
        }

        for (int t = 1; t < border_thickness; ++t) {
            x = radius - t;
            y = 0;
            err = 0;

            while (x >= y) {
                point(frame_buffer, x_center + x, y_center + y, color);
                point(frame_buffer, x_center + y, y_center + x, color);
                point(frame_buffer, x_center - y, y_center + x, color);
                point(frame_buffer, x_center - x, y_center + y, color);
                point(frame_buffer, x_center - x, y_center - y, color);
                point(frame_buffer, x_center - y, y_center - x, color);
                point(frame_buffer, x_center + y, y_center - x, color);
                point(frame_buffer, x_center + x, y_center - y, color);

                if (err <= 0) {
                    y += 1;
                    err += 2 * y + 1;
                }
                if (err > 0) {
                    x -= 1;
                    err -= 2 * x + 1;
                }
            }
        }
    }

    auto circle_filled(const FrameBufferInfo* frame_buffer, uint32_t x_center, uint32_t y_center, uint32_t radius, uint32_t color) -> void {
        int x = radius;
        int y = 0;
        int err = 0;

        while (x >= y) {
            for (int dx = -x; dx <= x; ++dx) {
                point(frame_buffer, x_center + dx, y_center + y, color);
                point(frame_buffer, x_center + dx, y_center - y, color);
            }
            for (int dx = -y; dx <= y; ++dx) {
                point(frame_buffer, x_center + dx, y_center + x, color);
                point(frame_buffer, x_center + dx, y_center - x, color);
            }

            if (err <= 0) {
                y += 1;
                err += 2 * y + 1;
            }
            if (err > 0) {
                x -= 1;
                err -= 2 * x + 1;
            }
        }
    }

    auto line(const FrameBufferInfo* frame_buffer, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t thickness, uint32_t color) -> void {
        int dx = abs((int)x1 - (int)x0);
        int dy = abs((int)y1 - (int)y0);
        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;
        int err = (dx > dy ? dx : -dy) / 2;
        int e2;

        while (true) {
            box_filled(frame_buffer, x0 - thickness / 2, y0 - thickness / 2, x0 + thickness / 2, y0 + thickness / 2, color);

            if (x0 == x1 && y0 == y1) break;
            e2 = err;
            if (e2 > -dx) { err -= dy; x0 += sx; }
            if (e2 < dy) { err += dx; y0 += sy; }
        }
    }
}