#ifndef S_FRAME_FLIGHT_RESOURCE_H
#define S_FRAME_FLIGHT_RESOURCE_H

#include <cstdint>

struct FrameFlightResource {
    FrameFlightResource(uint32_t frameFlightIndex)
        : frameFlightIndex(frameFlightIndex) {}
    uint32_t frameFlightIndex = 0;
};

#endif