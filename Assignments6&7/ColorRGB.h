#pragma once


struct ColorRGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    ColorRGB() : r(0), g(0), b(0) {}
    ColorRGB(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {}

    static const int maxColorComponent = 255;

    ColorRGB& operator=(const ColorRGB& color) {
        r = color.r;
        g = color.g;
        b = color.b;
        return *this;
    }
};
