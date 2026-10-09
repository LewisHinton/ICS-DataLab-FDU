/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 郭晟  25803050353
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.
  4. Interprets integer expressions using the Data Lab 32-bit bit-vector
     model: results outside the signed range retain their low 32 bits.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
#include "bits.h"

// P1
/* 
 * signMask - return a mask with only the most significant bit set (0x80000000)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int signMask(void) {
//对于这个32位的系统，mask应该为(0x80000000,即1在最高位，直接对1左移31位即可)
  return 1<<31;
}

// P2
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1, bitXor(7, 7) = 0
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 2
 */

int bitXor(int x, int y) {
  int atleastone = ~((~x)&(~y));
  int Bothone = x&y;
	return atleastone & (~Bothone);
}

// P3
/*
 * negativePart - return -x if x < 0, otherwise return 0
 *   Examples: negativePart(-10) = 10, negativePart(5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
int negativePart(int x){
  int neg = ~x +1;
  int mask = x>>31;

  return  neg & mask;
}


// P4
/*
 * copyByteWithin - copy byte src of x to byte dst, leaving all other bytes unchanged
 *   Bytes are numbered from 0 (least significant) to 3 (most significant).
 *   You can assume 0 <= src <= 3 and 0 <= dst <= 3.
 *   Example: copyByteWithin(0x11223344, 0, 2) = 0x11443344
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int copyByteWithin(int x, int src, int dst) {
    int srcShift = src << 3;
    int dstShift = dst << 3;
    int byte = (x >> srcShift) & 0xFF;
    int mask = 0xFF << dstShift;

    return (x & ~mask) | (byte << dstShift);
}

// P5
/* 
 * logicalShift - shift x to the right by n bits, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int logicalShift(int x, int n) {
  int mask = ~(((1<<31)>>n)<<1);
  return (x>>n)& mask;
}

// P6
/*
 * swapNibblePairs - swap the low and high 4 bits within each byte of x
 *   Examples: swapNibblePairs(0xAB) = 0xBA
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 18
 *   Rating: 4
 */
int swapNibblePairs(int x) {
  int mask = 0x0F;
  mask = mask | mask<<8;
  mask = mask | mask <<16;
  

  return (x & mask)<<4 | ((x>>4)&mask);
}

// P7
/*
 * secondLowestZeroBit - return a mask that marks the position of the second least significant 0 bit
 *   Examples: secondLowestZeroBit(0xFFFFFFFA) = 0x4, secondLowestZeroBit(0x7FFFFFFF) = 0
 *             secondLowestZeroBit(-1) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 4
 */
int secondLowestZeroBit(int x) {
  int filled = x | (x+1);//消除了第一个0，我们要求的第二个零对于filled来说就是第一个零了
  return  ~filled & (filled+1);
}

// P8
/*
 * oddParity - return the odd parity bit of x, that is,
 *      when the number of 1s in the binary representation of x is even, then the return 1, otherwise return 0.
 *   Examples: oddParity(5) = 1, oddParity(7) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 56
 *   Rating: 5
 */
int oddParity(int x) {
  int y = x ^ (x>>16);
  int z = y ^(y>>8);
  int f = z^ (z>>4);
  int g = f ^(f>>2);
  int p = g ^(g>>1);

  return !(p&1);
}

// P9
/* 
 * rotateRightBits - rotate x to right by n bits
 *   you can assume n >= 0
 *   Examples: rotateRightBits(0x12345678, 8) = 0x78123456
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 5
 */
int rotateRightBits(int x, int n) {
  int k = n&31;
  int tail = (x >> k) & ~(((1<<31)>>k)<<1);
  int head = (x & ~(~0<<k))<<((~k+1)&31);
  return tail | head;
}

// P10
/*
 * roundEvenPow2 - round nonnegative x to the nearest multiple of 2^n.
 *   If x is exactly halfway between two multiples, choose the multiple whose
 *   quotient by 2^n is even.
 *   You can assume 0 <= x <= 0x3fffffff and 1 <= n <= 16.
 *   Examples: roundEvenPow2(10, 2) = 8, roundEvenPow2(14, 2) = 16
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 5
 */
int roundEvenPow2(int x, int n) {
  int q = x >> n;
  int r = x & ~(~0<<n);
  int half = 1<<(n+~0);
  int guard = !!(r&half);
  int sticky = !!(r&(half+~0));
  int inc = guard & (sticky | (q&1));



  return (q+inc)<<n;
}

// P11
/* 
 * midpointTowardFirst - return the exact mathematical midpoint (x+y)/2
 *   without overflow. If the exact midpoint lies halfway between two
 *   integers, choose the adjacent integer that is closer to the first
 *   argument x.
 *   Examples: midpointTowardFirst(4, 7) = 5,
 *             midpointTowardFirst(7, 4) = 6
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 32
 *   Rating: 5
 */
int midpointTowardFirst(int x, int y) {
  /* Compute the floor midpoint, then round an odd sum toward x. */
  int differentBits = x ^ y;
  int differentSigns = differentBits >> 31;
  int xGreater = (differentSigns & (y >> 31)) |
                 (~differentSigns & ((y + ~x + 1) >> 31));
  return (x & y) + (differentBits >> 1) +
         (differentBits & xGreater & 1);
}


// P12
/* 
 * isBetweenEitherOrder - return 1 when x lies in the inclusive interval whose
 *   endpoints are a and b. The endpoints may be given in either order.
 *   Example: isBetweenEitherOrder(5, 8, 3) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 48
 *   Rating: 7
 */
int isBetweenEitherOrder(int x, int a, int b) {
  /* Subtraction determines order only when the operands have equal signs. */
  int signX = x >> 31;
  int differentA = (x ^ a) >> 31;
  int differentB = (x ^ b) >> 31;
  int belowA = (differentA & signX) |
               (~differentA & ((x + ~a + 1) >> 31));
  int belowB = (differentB & signX) |
               (~differentB & ((x + ~b + 1) >> 31));
  return !!(belowA ^ belowB) | !(x ^ a) | !(x ^ b);
}

// P13
/* 
 * mul5Sat - return x*5, and if x*5 overflow, change the result to 
 * INT_MAX(0x7fffffff) or INT_MIN(0x80000000) correspondingly
 *   Examples: mul5Sat(1) = 0x5, mul5Sat(0x40000000) = 0x7fffffff
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 7
 */
int mul5Sat(int x) {
  /* Every intermediate sign change implies that the exact 5*x overflows. */
  int twice = x + x;
  int four = twice + twice;
  int five = four + x;
  int overflow = ((x ^ twice) | (twice ^ four) | (four ^ five)) >> 31;
  int limit = (1 << 31) ^ ~(x >> 31);
  return (overflow & limit) | (~overflow & five);
}

// P14
/* 
 * classifyAdd3 - classify the exact mathematical sum x+y+z.
 *   Return 1 if the sum is greater than INT_MAX, -1 if it is less than
 *   INT_MIN, and 0 otherwise. You may not use a wider integer type.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 52
 *   Rating: 7
 */
int classifyAdd3(int x, int y, int z) {
  /* Each signed overflow contributes +1 or -1 times 2^32 to the exact sum.
     Opposite contributions cancel; two contributions of the same sign
     cannot occur in these two consecutive additions. */
  int sum = x + y;
  int total = sum + z;
  int overflowXY = (~(x ^ y) & (x ^ sum)) >> 31;
  int overflowZ = (~(sum ^ z) & (sum ^ total)) >> 31;
  int correctionXY = overflowXY & ((x >> 31) | 1);
  int correctionZ = overflowZ & ((sum >> 31) | 1);
  return correctionXY + correctionZ;
}

// P15
/*
 * floatScaleThreeHalves - Return bit-level equivalent of expression f*3/2 for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   Use round-to-nearest-even. Preserve the sign of both +0 and -0.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer / unsigned operations incl. ||, &&. also if, while
 *   Max ops: 60
 *   Rating: 7
 */
unsigned floatScaleThreeHalves(unsigned uf) {
  unsigned sign = uf & 0x80000000u;
  unsigned exponent = (uf >> 23) & 0xff;
  unsigned significand = uf & 0x7fffff;
  unsigned shift = 1;
  unsigned rounded;
  unsigned remainder;
  unsigned half;

  if (exponent == 255)
    return uf;
  if (exponent != 0)
    significand = significand | 0x800000;

  /* Keep 3*M exact, then divide by 2 (or 4 after normalization) once. */
  significand = significand + (significand << 1);
  if (significand >= 0x2000000) {
    shift = 2;
    exponent = exponent + 1;
  }
  rounded = significand >> shift;
  remainder = significand & ((1u << shift) - 1);
  half = 1u << (shift - 1);
  rounded = rounded + ((remainder > half) ||
                       ((remainder == half) && (rounded & 1)));

  /* A subnormal rounding up to 0x800000 already encodes a normal value. */
  if (exponent == 0)
    return sign | rounded;
  if (rounded >= 0x1000000) {
    rounded = rounded >> 1;
    exponent = exponent + 1;
  }
  if (exponent >= 255)
    return sign | 0x7f800000;
  return sign | (exponent << 23) | (rounded & 0x7fffff);
}

// P16
/* 
 * floatRoundEven - round the floating-point value represented by uf to the
 *   nearest integer, with halfway cases rounded to the even integer. Return
 *   the bit-level representation of that integer as a single-precision float.
 *   If rounding produces zero, preserve the input sign; thus a negative
 *   value that rounds to zero returns -0. When uf is NaN or infinity,
 *   return uf unchanged.
 *   Legal ops: Any integer / unsigned operations incl. ||, &&. also if, while
 *   Max ops: 65
 *   Rating: 10
 */
unsigned floatRoundEven(unsigned uf) {
  unsigned sign = uf & 0x80000000u;
  unsigned exponent = (uf >> 23) & 0xff;
  unsigned shift;
  unsigned mask;
  unsigned remainder;
  unsigned half;
  unsigned rounded;

  /* Large finite floats are already integral; also preserve NaNs/infinity. */
  if (exponent >= 150)
    return uf;
  if (exponent < 126)
    return sign;
  if (exponent == 126) {
    if (uf & 0x7fffff)
      return sign | 0x3f800000;
    return sign;
  }

  shift = 150 - exponent;
  mask = (1u << shift) - 1;
  remainder = uf & mask;
  half = 1u << (shift - 1);
  rounded = uf & ~mask;
  /* For exponent 127, its low bit supplies the implicit leading 1. */
  if ((remainder > half) ||
      ((remainder == half) && ((rounded >> shift) & 1)))
    rounded = rounded + (1u << shift);
  return rounded;
}

// P17
/*
 * float_i2f - Return bit-level equivalent of expression (float) x.
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer / unsigned operations incl. ||, &&. also if, while
 *   Max ops: 40
 *   Rating: 10
 */
unsigned float_i2f(int x) {
  unsigned sign = x & 0x80000000u;
  unsigned magnitude = x;
  unsigned fraction;
  unsigned remainder;
  int highest = 31;

  if (x == 0)
    return 0;
  /* Unsigned negation also handles INT_MIN without signed overflow. */
  if (sign)
    magnitude = ~magnitude + 1;
  while (!(magnitude & 0x80000000u)) {
    magnitude = magnitude << 1;
    highest = highest - 1;
  }

  fraction = (magnitude >> 8) & 0x7fffff;
  remainder = magnitude & 0xff;
  if ((remainder > 0x80) ||
      ((remainder == 0x80) && (fraction & 1)))
    fraction = fraction + 1;
  /* Addition lets a rounding carry advance the exponent automatically. */
  return sign | (((highest + 127) << 23) + fraction);
}



// P18
/*
 * bitCount - return count of number of 1's in the binary representation of x
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 10
 */
int bitCount(int x) {
  /* SWAR: count within 2-bit, 4-bit, then 8-bit lanes before summing. */
  int pairs = 0x55 | (0x55 << 8);
  int nibbles = 0x33 | (0x33 << 8);
  int bytes = 0x0f | (0x0f << 8);
  pairs = pairs | (pairs << 16);
  nibbles = nibbles | (nibbles << 16);
  bytes = bytes | (bytes << 16);
  x = (x & pairs) + ((x >> 1) & pairs);
  x = (x & nibbles) + ((x >> 2) & nibbles);
  x = (x + (x >> 4)) & bytes;
  x = x + (x >> 8);
  x = x + (x >> 16);
  return x & 0x3f;
}

// P19
/*
 * bitReverse - Reverse bits in an 32-bit integer
 *   Examples: bitReverse(0x80000004) = 0x20000001
 *             bitReverse(0x7FFFFFFF) = 0xFFFFFFFE
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 34
 *   Rating: 10
 */
int bitReverse(int x)
{
  /* Derive finer alternating masks from 0x00ff00ff to save operations. */
  int bytes = 0xff | (0xff << 16);
  int nibbles = bytes ^ (bytes << 4);
  int pairs = nibbles ^ (nibbles << 2);
  int bits = pairs ^ (pairs << 1);
  int half = 0xff | (0xff << 8);
  x = ((x >> 1) & bits) | ((x & bits) << 1);
  x = ((x >> 2) & pairs) | ((x & pairs) << 2);
  x = ((x >> 4) & nibbles) | ((x & nibbles) << 4);
  x = ((x >> 8) & bytes) | ((x & bytes) << 8);
  return (x << 16) | ((x >> 16) & half);
}
