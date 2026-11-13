#include "BrightPixelRow.hpp"

BrightPixelRow::BrightPixelRow() : PixelRow() {}

BrightPixelRow::BrightPixelRow(int count, int initVal, int extraVal) : PixelRow(count, initVal, extraVal) {}

BrightPixelRow::BrightPixelRow(const BrightPixelRow& other) : PixelRow(other) {}

BrightPixelRow::~BrightPixelRow() {}

BrightPixelRow& BrightPixelRow::operator=(const BrightPixelRow& other) {
    if (this != &other) {
        PixelRow::operator=(other);
    }
    return *this;
}

// TODO 5: Override luminanceSum() (Maximum 8 lines of implementation)
int BrightPixelRow::luminanceSum() const {
    return static_cast<int>(0);
}
