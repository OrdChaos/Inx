#include "../../include/lib.h"

auto abs(int8_t num) -> int8_t {
    return (num < 0) ? -num : num;
}

auto abs(int16_t num) -> int16_t {
    return (num < 0) ? -num : num;
}

auto abs(int32_t num) -> int32_t {
    return (num < 0) ? -num : num;
}

auto abs(int64_t num) -> int64_t {
    return (num < 0) ? -num : num;
}

auto memcpy(void* dest, const void* src, size_t n) -> void {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    for (size_t i = 0; i < n; ++i) {
        d[i] = s[i];
    }
}

auto memset(void* start, uint8_t value, uint64_t num) -> void {
    for (uint64_t i = 0; i < num; i++){
        *(uint8_t*)((uint64_t)start + i) = value;
    }
}

auto swap(auto &a, auto &b) -> void {
    auto c = a;
    a = b;
    b = c;
}

auto outb(uint16_t port, uint8_t value) -> void {
    asm volatile ("outb %0, %1" :: "a"(value), "Nd"(port));
}

auto inb(uint16_t port) -> uint8_t {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

auto itoa(int32_t value, char *str, int base) -> void {
    char *p = str;
    char *p1, *p2;
    uint32_t uvalue = value;

    if (base == 10 && value < 0) {
        *p++ = '-';
        uvalue = -value;
    }

    do {
        int digit = uvalue % base;
        *p++ = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
        uvalue /= base;
    } while (uvalue);

    *p = '\0';

    for (p1 = str + (value < 0 ? 1 : 0), p2 = p - 1; p1 < p2; p1++, p2--) {
        char temp = *p1;
        *p1 = *p2;
        *p2 = temp;
    }
}

auto ltoa(int64_t value, char *str, int base) -> void {
    char *p = str;
    char *p1, *p2;
    uint64_t uvalue = value;

    if (base == 10 && value < 0) {
        *p++ = '-';
        uvalue = -value;
    }

    do {
        int digit = uvalue % base;
        *p++ = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
        uvalue /= base;
    } while (uvalue);

    *p = '\0';

    for (p1 = str + (value < 0 ? 1 : 0), p2 = p - 1; p1 < p2; p1++, p2--) {
        char temp = *p1;
        *p1 = *p2;
        *p2 = temp;
    }
}

auto vsprintf(char *buf, const char *fmt, va_list args) -> int32_t {
    char *p;
    char temp[32]; // Buffer for integer conversion
    bool is_long = false;
    bool is_long_long = false;

    for (p = buf; *fmt != '\0'; fmt++) {
        if (*fmt != '%') {
            *p++ = *fmt;
            continue;
        }

        fmt++;
        // Check for length modifiers
        if (*fmt == 'l') {
            fmt++;
            if (*fmt == 'l') {
                is_long_long = true;
                fmt++;
            } else {
                is_long = true;
            }
        }

        switch (*fmt) {
            case 'c':
                *p++ = (char)va_arg(args, int);
                break;

            case 's':
                for (char *str = va_arg(args, char*); *str != '\0'; str++) {
                    *p++ = *str;
                }
                break;

            case 'd':
                if (is_long_long) {
                    int64_t ll = va_arg(args, int64_t);
                    ltoa(ll, temp, 10);
                } else if (is_long) {
                    long l = va_arg(args, long);
                    ltoa(l, temp, 10);
                } else {
                    int i = va_arg(args, int);
                    itoa(i, temp, 10);
                }
                for (char *str = temp; *str != '\0'; str++) {
                    *p++ = *str;
                }
                is_long = false;
                is_long_long = false;
                break;

            case 'b':
                {
                    int i = va_arg(args, int);
                    itoa(i, temp, 2);
                    for (char *str = temp; *str != '\0'; str++) {
                        *p++ = *str;
                    }
                }
                break;

            case 'o':
                {
                    int i = va_arg(args, int);
                    itoa(i, temp, 8);
                    for (char *str = temp; *str != '\0'; str++) {
                        *p++ = *str;
                    }
                }
                break;

            case 'x':
                if (is_long_long) {
                    uint64_t ull = va_arg(args, uint64_t);
                    ltoa(ull, temp, 16);
                } else if (is_long) {
                    unsigned long ul = va_arg(args, unsigned long);
                    ltoa(ul, temp, 16);
                } else {
                    unsigned int ui = va_arg(args, unsigned int);
                    itoa(ui, temp, 16);
                }
                for (char *str = temp; *str != '\0'; str++) {
                    *p++ = *str;
                }
                is_long = false;
                is_long_long = false;
                break;

            default:
                *p++ = *fmt;
                break;
        }
    }

    *p = '\0';
    return p - buf;
}

auto sprintf(char *buf, const char *fmt, ...) -> int32_t {
    va_list args;
    va_start(args, fmt);
    int result = vsprintf(buf, fmt, args);
    va_end(args);
    return result;
}