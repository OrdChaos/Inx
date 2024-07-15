#ifndef LIB_H
#define LIB_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include "lib/queue.h"
#include "lib/vector.h"

auto abs(int8_t num) -> int8_t;
auto abs(int16_t num) -> int16_t;
auto abs(int32_t num) -> int32_t;
auto abs(int64_t num) -> int64_t;

auto memcpy(void *dest, const void *src, size_t n) -> void;
auto memset(void* start, uint8_t value, uint64_t num) -> void;
auto swap(auto &a, auto &b) -> void;

auto outb(uint16_t port, uint8_t value) -> void;
auto inb(uint16_t port) -> uint8_t;

auto itoa(int32_t value, char *str, int base) -> void;
auto ltoa(int64_t value, char *str, int base) -> void;
auto vsprintf(char *buf, const char *fmt, va_list args) -> int32_t;
auto sprintf(char *buf, const char *fmt, ...) -> int32_t;

#endif