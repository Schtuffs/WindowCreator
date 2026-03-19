#pragma once

#include <cstdint>

#include "Defines.h"

typedef struct Colour {
    uint8_t r, g, b, a;
    
    Colour(uint8_t greyScale = COLOUR_DEFAULT);
    Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    ~Colour() = default;
} Colour;

