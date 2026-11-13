#ifndef PIXELROW_HPP
#define PIXELROW_HPP

#include <iostream>
using namespace std;

class PixelRow {
protected:
    int* pixels;
    int width;
    int brightnessShift;

public:
    PixelRow();
    PixelRow(int count, int initVal, int extraVal);
    PixelRow(const PixelRow& other);
    virtual ~PixelRow();
    PixelRow& operator=(const PixelRow& other);

    int getPixel(int index) const;
    virtual int luminanceSum() const;
};

#endif
