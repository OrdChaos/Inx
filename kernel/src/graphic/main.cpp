#include "../../include/graphic/graphic.h"
#include "../../include/graphic/CyrAsia-Terminus16.h"

namespace graphic {
    auto init() -> bool {
        return print::psf1::loadFont(font_bytes, font_bytes_len);
    }
}