#pragma once
// Attack Shark X8 Plus passive Xenta status reports (source: cyberps96/attack-shark-ctl).
// Platform-independent so packet decoding can be tested without Windows HID hardware.
#include <cstddef>
#include <cstdint>

namespace Xenta {
constexpr std::uint8_t kX8PlusCode = 0x06;
struct Battery {
    int percent = -1;
    bool charging = false;
};
inline bool DecodeX8Plus(const std::uint8_t* frame, std::size_t size, Battery& out) {
    if (!frame || size < 5 || frame[0] != 0x03 || frame[1] != kX8PlusCode || frame[2] != 0x40)
        return false;
    const unsigned state = frame[3], percent = frame[4];
    // 02 = charging. 03 = USB / full (not necessarily active charging on the receiver).
    // 00 / 01 = discharging. Never turn an invalid level into 100%.
    if (state > 3 || percent > 100) return false;
    Battery decoded;
    decoded.percent = static_cast<int>(percent);
    decoded.charging = state == 2;
    out = decoded;
    return true;
}
}  // namespace Xenta
