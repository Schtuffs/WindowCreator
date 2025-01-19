#pragma once

#include "Defines.h"

class Colour {
private:
    unsigned char m_r, m_g, m_b, m_a;

public:
    Colour(int greyScale = COLOUR_DEFAULT);
    Colour(int r, int g, int b, int a = 255);
    ~Colour();

    bool SetR(int r);
    bool SetG(int g);
    bool SetB(int b);
    bool SetA(int a);

    int GetR();
    int GetG();
    int GetB();
    int GetA();
};

