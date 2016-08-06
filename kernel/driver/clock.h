#ifndef __CLOCK_H
#define __CLOCK_H
#include "clock.hpp"

namespace os {
namespace clock {

namespace hidden {

constexpr uint16_t PIT_PORT_CHANNEL0 = 0x40;
constexpr uint16_t PIT_PORT_MODE = PIT_PORT_CHANNEL0 + 3;
constexpr uint32_t PIT_FREQ = 1193182;
constexpr uint32_t PIT_DEMANDED_INTERVAL = 1000;
constexpr uint8_t PIT_SELECTOR_0 = 0x00;
constexpr uint8_t PIT_RATE_GEN = 0x04;
constexpr uint8_t PIT_16BIT = 0x30;

} // hidden

void initClock();
extern Clock clock;

} // clock
} // os

#endif