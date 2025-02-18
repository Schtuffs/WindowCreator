#pragma once

#include "Defines.h"

class Colour {
private:
    unsigned char m_r, m_g, m_b, m_a;

public:
    Colour(int greyScale = COLOUR_DEFAULT);
    Colour(int r, int g, int b, int a = 255);
    ~Colour();

    bool setR(int r);
    bool setG(int g);
    bool setB(int b);
    bool setA(int a);

    int getR();
    int getG();
    int getB();
    int getA();
};

