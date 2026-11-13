#include <iostream>
#include "PixelRow.hpp"
#include "BrightPixelRow.hpp"

using namespace std;

// Lightweight heap allocation tracker using only <iostream> and C++11 primitives (no size_t, no extra headers)
static char g_heapPool[65536];
static int g_poolOffset = 0;
static int g_activeAllocs = 0;
static int g_totalAllocs = 0;

void* operator new(decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    // 8-byte alignment
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void* operator new[](decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void operator delete(void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete(void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

bool approxEqual(double a, double b) {
    double diff = a - b;
    if (diff < 0.0) diff = -diff;
    return diff < 0.001;
}

int main() {
    int passed = 0;
    int failed = 0;

    // Expected reference values computed dynamically
    // Array of 3 elements initialized as: initVal + 0, initVal + 1, initVal + 2
    int initV = static_cast<int>(100);
    int extV = static_cast<int>(25);

    // --- PARENT CLASS TEST CASES (1 to 5) ---
    // TC 1: Default constructor & empty compute
    {
        PixelRow pDef;
        if (approxEqual(static_cast<double>(pDef.luminanceSum()), 0.0) &&
            approxEqual(static_cast<double>(pDef.getPixel(0)), 0.0)) {
            cout << "[PASS] TC 01: PixelRow default constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 01: PixelRow default constructor" << endl;
            ++failed;
        }
    }

    // TC 2: Parameterized constructor & base compute()
    {
        PixelRow p1(3, initV, extV);
        bool ok = approxEqual(static_cast<double>(p1.getPixel(0)), static_cast<double>(initV)) &&
                  approxEqual(static_cast<double>(p1.getPixel(2)), static_cast<double>(initV + static_cast<int>(2)));
        // Verify compute() is non-trivial when initV > 0
        if (ok && p1.getPixel(1) == static_cast<int>(initV + static_cast<int>(1))) {
            // Compare against a second identical instance
            PixelRow pCheck(3, initV, extV);
            if (approxEqual(static_cast<double>(p1.luminanceSum()), static_cast<double>(pCheck.luminanceSum())) &&
                !approxEqual(static_cast<double>(p1.luminanceSum()), 0.0)) {
                cout << "[PASS] TC 02: PixelRow parameterized constructor & luminanceSum()" << endl;
                ++passed;
            } else {
                cout << "[FAIL] TC 02: PixelRow luminanceSum() returned unexpected value" << endl;
                ++failed;
            }
        } else {
            cout << "[FAIL] TC 02: PixelRow element initialization mismatch" << endl;
            ++failed;
        }
    }

    // TC 3: Parent Copy Constructor (Deep Copy verification)
    {
        PixelRow orig(3, initV, extV);
        PixelRow copyObj(orig);
        if (approxEqual(static_cast<double>(copyObj.luminanceSum()), static_cast<double>(orig.luminanceSum())) &&
            approxEqual(static_cast<double>(copyObj.getPixel(1)), static_cast<double>(orig.getPixel(1))) &&
            !approxEqual(static_cast<double>(copyObj.luminanceSum()), 0.0)) {
            cout << "[PASS] TC 03: PixelRow copy constructor deep copy" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 03: PixelRow copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 4: Parent Copy Assignment Operator & Self-Assignment
    {
        PixelRow a(3, initV, extV);
        PixelRow b(2, initV, extV);
        b = a;
        PixelRow* selfPtr = &b;
        b = *selfPtr; // Self-assignment check
        if (approxEqual(static_cast<double>(b.luminanceSum()), static_cast<double>(a.luminanceSum())) &&
            approxEqual(static_cast<double>(b.getPixel(2)), static_cast<double>(a.getPixel(2))) &&
            !approxEqual(static_cast<double>(b.luminanceSum()), 0.0)) {
            cout << "[PASS] TC 04: PixelRow copy assignment & self-assignment" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 04: PixelRow copy assignment failed" << endl;
            ++failed;
        }
    }

    // TC 5: Parent Destructor Heap Release Check
    {
        int before = g_activeAllocs;
        {
            PixelRow temp(4, initV, extV);
        }
        int after = g_activeAllocs;
        if (before == after && g_totalAllocs > 0) {
            cout << "[PASS] TC 05: PixelRow destructor released heap memory" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 05: PixelRow destructor leaked heap memory" << endl;
            ++failed;
        }
    }

    // --- CHILD CLASS TEST CASES (6 to 10) ---
    // TC 6: Child Parameterized Construction & Overridden compute()
    {
        PixelRow baseObj(3, initV, extV);
        BrightPixelRow childObj(3, initV, extV);
        if (!approxEqual(static_cast<double>(childObj.luminanceSum()), 0.0) &&
            !approxEqual(static_cast<double>(childObj.luminanceSum()), static_cast<double>(baseObj.luminanceSum()))) {
            cout << "[PASS] TC 06: BrightPixelRow overridden luminanceSum()" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 06: BrightPixelRow overridden luminanceSum() failed" << endl;
            ++failed;
        }
    }

    // TC 7: Polymorphic Dispatch via Base Pointer
    {
        BrightPixelRow directChild(3, initV, extV);
        PixelRow* polyPtr = &directChild;
        if (!approxEqual(static_cast<double>(polyPtr->luminanceSum()), 0.0) &&
            approxEqual(static_cast<double>(polyPtr->luminanceSum()), static_cast<double>(directChild.luminanceSum()))) {
            cout << "[PASS] TC 07: BrightPixelRow polymorphic dispatch via PixelRow*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 07: BrightPixelRow polymorphic dispatch failed" << endl;
            ++failed;
        }
    }

    // TC 8: Child Copy Constructor
    {
        BrightPixelRow c1(3, initV, extV);
        BrightPixelRow c2(c1);
        if (approxEqual(static_cast<double>(c1.luminanceSum()), static_cast<double>(c2.luminanceSum())) &&
            !approxEqual(static_cast<double>(c2.luminanceSum()), 0.0)) {
            cout << "[PASS] TC 08: BrightPixelRow copy constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 08: BrightPixelRow copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 9: Child Copy Assignment Operator
    {
        BrightPixelRow c1(3, initV, extV);
        BrightPixelRow c2;
        c2 = c1;
        if (approxEqual(static_cast<double>(c1.luminanceSum()), static_cast<double>(c2.luminanceSum())) &&
            !approxEqual(static_cast<double>(c2.luminanceSum()), 0.0)) {
            cout << "[PASS] TC 09: BrightPixelRow copy assignment operator" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 09: BrightPixelRow copy assignment operator failed" << endl;
            ++failed;
        }
    }

    // TC 10: Virtual Destructor via Base Pointer Deletion
    {
        int before = g_activeAllocs;
        PixelRow* heapChild = new BrightPixelRow(4, initV, extV);
        delete heapChild;
        int after = g_activeAllocs;
        if (before == after) {
            cout << "[PASS] TC 10: BrightPixelRow virtual destructor via PixelRow*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 10: BrightPixelRow virtual destructor leaked memory" << endl;
            ++failed;
        }
    }

    cout << "========================================" << endl;
    cout << "FINAL SCORE: " << passed << " / 10 PASSED (" << failed << " FAILED)" << endl;
    if (g_activeAllocs == 0) {
        cout << "MEMORY LEAK REPORT: NO MEMORY LEAKS DETECTED (0 active allocations)." << endl;
    } else {
        cout << "MEMORY LEAK REPORT: WARNING! " << g_activeAllocs << " active heap allocation(s) leaked!" << endl;
    }
    cout << "========================================" << endl;

    return (failed == 0 && g_activeAllocs == 0) ? 0 : 1;
}
