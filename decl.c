#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TMin INT_MIN
#define TMax INT_MAX

#include "btest.h"
#include "bits.h"

test_rec test_set[] = {

    {"signMask", (funct_t)signMask, (funct_t)test_signMask, 0, "! ~ & ^ | + << >>", 2, 1, {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"bitXor", (funct_t)bitXor, (funct_t)test_bitXor, 2, "& ~", 8, 2, {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"negativePart", (funct_t)negativePart, (funct_t)test_negativePart, 1, "! ~ & ^ | + << >>", 6, 3, {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"copyByteWithin", (funct_t)copyByteWithin, (funct_t)test_copyByteWithin, 3, "! ~ & ^ | + << >>", 12, 4, {{TMin, TMax}, {0, 3}, {0, 3}}},
    {"logicalShift", (funct_t)logicalShift, (funct_t)test_logicalShift, 2, "! ~ & ^ | + << >>", 20, 4, {{TMin, TMax}, {0, 31}, {TMin, TMax}}},
    {"swapNibblePairs", (funct_t)swapNibblePairs, (funct_t)test_swapNibblePairs, 1, "! ~ & ^ | + << >>", 18, 4, {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"secondLowestZeroBit", (funct_t)secondLowestZeroBit, (funct_t)test_secondLowestZeroBit, 1, "! ~ & ^ | + << >>", 8, 4, {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"oddParity", (funct_t)oddParity, (funct_t)test_oddParity, 1, "! ~ & ^ | + << >>", 56, 5, {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"rotateRightBits", (funct_t)rotateRightBits, (funct_t)test_rotateRightBits, 2, "! ~ & ^ | + << >>", 16, 5, {{TMin, TMax}, {0, TMax}, {TMin, TMax}}},
    {"roundEvenPow2", (funct_t)roundEvenPow2, (funct_t)test_roundEvenPow2, 2, "! ~ & ^ | + << >>", 24, 5, {{0, 1073741823}, {1, 16}, {TMin, TMax}}},
    {"midpointTowardFirst", (funct_t)midpointTowardFirst, (funct_t)test_midpointTowardFirst, 2, "! ~ & ^ | + << >>", 32, 5, {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"isBetweenEitherOrder", (funct_t)isBetweenEitherOrder, (funct_t)test_isBetweenEitherOrder, 3, "! ~ & ^ | + << >>", 48, 7, {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"mul5Sat", (funct_t)mul5Sat, (funct_t)test_mul5Sat, 1, "! ~ & ^ | + << >>", 30, 7, {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"classifyAdd3", (funct_t)classifyAdd3, (funct_t)test_classifyAdd3, 3, "! ~ & ^ | + << >>", 52, 7, {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},

    // float
    {"floatScaleThreeHalves", (funct_t)floatScaleThreeHalves, (funct_t)test_floatScaleThreeHalves, 1, "$", 60, 7, {{1, 1}, {1, 1}, {1, 1}}},

    // honor part
    {"floatRoundEven", (funct_t)floatRoundEven, (funct_t)test_floatRoundEven, 1, "$", 65, 10, {{1, 1}, {1, 1}, {1, 1}}},
    {"float_i2f", (funct_t)float_i2f, (funct_t)test_float_i2f, 1, "$", 40, 10, {{1, 1}, {1, 1}, {1, 1}}},
    {"bitCount", (funct_t)bitCount, (funct_t)test_bitCount, 1, "! ~ & ^ | + << >>", 40, 10, {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},
    {"bitReverse", (funct_t)bitReverse, (funct_t)test_bitReverse, 1, "! ~ & ^ | + << >>", 34, 10, {{TMin, TMax}, {TMin, TMax}, {TMin, TMax}}},

    {"", NULL, NULL, 0, "", 0, 0, {{0, 0}, {0, 0}, {0, 0}}}};
