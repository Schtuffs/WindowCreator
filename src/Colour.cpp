#include "Colour.h"

// ----- Creation / Destruction -----

Colour::Colour(int greyscale) {
    if (greyscale > 255) {
        greyscale = COLOUR_DEFAULT;
    }
    this->m_r = greyscale;
    this->m_g = greyscale;
    this->m_b = greyscale;
    this->m_a = 255;
}

Colour::Colour(int r, int g, int b, int a) {
    if (r > 255) {
        r = COLOUR_DEFAULT;
    }
    if (g > 255) {
        g = COLOUR_DEFAULT;
    }
    if (b > 255) {
        b = COLOUR_DEFAULT;
    }
    if (a > 255) {
        a = COLOUR_DEFAULT;
    }

    this->m_r = r;
    this->m_g = g;
    this->m_b = b;
    this->m_a = a;
}

Colour::~Colour() {
    // Nothing todo
}



// ----- Setters -----

bool Colour::SetR(int r) {
    if (0 > r || r > 255) {
        return false;
    }

    this->m_r = r;
    return true; 
}

bool Colour::SetG(int g) {
    if (0 > g || g > 255) {
        return false;
    }

    this->m_g = g;
    return true; 
}

bool Colour::SetB(int b) {
    if (0 > b || b > 255) {
        return false;
    }

    this->m_b = b;
    return true; 
}

bool Colour::SetA(int a) {
    if (0 > a || a > 255) {
        return false;
    }

    this->m_a = a;
    return true; 
}


// ----- Getters -----

int Colour::GetR() {
    return this->m_r;
}

int Colour::GetG() {
    return this->m_g;
}

int Colour::GetB() {
    return this->m_b;
}

int Colour::GetA() {
    return this->m_a;
}

