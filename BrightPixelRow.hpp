#ifndef BRIGHTPIXELROW_HPP
#define BRIGHTPIXELROW_HPP

#include "PixelRow.hpp"

class BrightPixelRow : public PixelRow {
public:
    BrightPixelRow();
    BrightPixelRow(int count, int initVal, int extraVal);
    BrightPixelRow(const BrightPixelRow& other);
    virtual ~BrightPixelRow();
    BrightPixelRow& operator=(const BrightPixelRow& other);

    virtual int luminanceSum() const override;
};

#endif
