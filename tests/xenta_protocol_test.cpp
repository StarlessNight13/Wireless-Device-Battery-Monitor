#include "../src/drivers/xenta_protocol.h"
#include <cassert>
#include <cstdint>
#include <cstdio>

int main() {
    const std::uint8_t discharging[] = {0x03, 0x06, 0x40, 0x01, 87};
    Xenta::Battery b;
    assert(Xenta::DecodeX8Plus(discharging, sizeof(discharging), b));
    assert(b.percent == 87 && !b.charging);

    const std::uint8_t charging[] = {0x03, 0x06, 0x40, 0x02, 42};
    assert(Xenta::DecodeX8Plus(charging, sizeof(charging), b));
    assert(b.percent == 42 && b.charging);  // Never turn charging into 100%.

    const std::uint8_t full[] = {0x03, 0x06, 0x40, 0x03, 100};
    assert(Xenta::DecodeX8Plus(full, sizeof(full), b));
    assert(b.percent == 100 && !b.charging);

    const std::uint8_t empty[] = {0x03, 0x06, 0x40, 0x00, 0};
    assert(Xenta::DecodeX8Plus(empty, sizeof(empty), b));
    assert(b.percent == 0 && !b.charging);

    // Invalid frames leave the last valid state untouched.
    std::uint8_t invalid[] = {0x03, 0x06, 0x40, 0x01, 101};
    assert(!Xenta::DecodeX8Plus(invalid, sizeof(invalid), b));
    assert(b.percent == 0 && !b.charging);
    invalid[4] = 55;
    invalid[3] = 0xFF;
    assert(!Xenta::DecodeX8Plus(invalid, sizeof(invalid), b));
    invalid[3] = 1;
    invalid[1] = 0x55;  // X11 uses the existing generic Xenta driver.
    assert(!Xenta::DecodeX8Plus(invalid, sizeof(invalid), b));
    invalid[1] = 0x06;
    invalid[2] = 0x00;
    assert(!Xenta::DecodeX8Plus(invalid, sizeof(invalid), b));
    invalid[2] = 0x40;
    invalid[0] = 0x00;
    assert(!Xenta::DecodeX8Plus(invalid, sizeof(invalid), b));
    assert(!Xenta::DecodeX8Plus(discharging, 4, b));
    assert(!Xenta::DecodeX8Plus(nullptr, 5, b));
    std::puts("Xenta X8 Plus protocol tests passed.");
}
