#include "PixelRow.hpp"

PixelRow::PixelRow() : pixels(nullptr), width(0), brightnessShift(25) {}

PixelRow::PixelRow(int count, int initVal, int extraVal) : pixels(nullptr), width(count), brightnessShift(extraVal) {
    if (width > 0) {
        pixels = new int[width];
        for (int i = 0; i < width; ++i) {
            pixels[i] = initVal + static_cast<int>(i);
        }
    } else {
        width = 0;
    }
}

// TODO 1: Complete the Copy Constructor (Deep Copy)
PixelRow::PixelRow(const PixelRow& other) {
    (void)other;
}

// TODO 2: Complete the Destructor (Release DMA)
PixelRow::~PixelRow() {
}

// TODO 3: Complete the Copy Assignment Operator (Deep Copy + Self-Assignment Guard)
PixelRow& PixelRow::operator=(const PixelRow& other) {
    (void)other;
    return *this;
}

int PixelRow::getPixel(int index) const {
    if (index >= 0 && index < width && pixels != nullptr) {
        return pixels[index];
    }
    return static_cast<int>(0);
}

// TODO 4: Implement luminanceSum() (Maximum 8 lines of implementation)
int PixelRow::luminanceSum() const {
    return static_cast<int>(0);
}
