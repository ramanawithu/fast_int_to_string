
#ifndef _FAST_INT_TO_STRING_
#define _FAST_INT_TO_STRING_

#include <string>
#include <cstdint>

// An argument of second signature is pointer type
#define FILE_SCOPE static

// Matrix of precomputed ASCII form of digits
static
const char digits_in_ascii[] = {
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899"
};

// Wrapper over compiler intrensic to avoid forbidden cases
//      x86 (x = 0 is not expected by builtin __builtin_clzll even-though it returns proper value)
// returns number of leading zeros in binary representation of input integer.
FILE_SCOPE
inline
uint32_t count_leading_zeros(uint64_t x) {
#ifdef __x86_64__
    if (0 == x)
        return 64;
    return __builtin_clzll(x);
#else
    // TODO: Leverage loop un-rolled version of 'counting leading zeros'
    #error "Builtin intrinisic is not defined on this platform"
#endif
}

// Returns number of decimal digits of 'x'.
// Based on number of leading zeros, we index into two different tables and one comparison operation.
// Reference: Hacker's Delight
FILE_SCOPE
inline
uint32_t log10_characterstic(uint64_t x) {
    static const unsigned char clz_to_no_of_decimal_digits[] = {
    //   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15     <----- Index into this array
        19, 19, 19, 19, 18, 18, 18, 17, 17, 17, 16, 16, 16, 16, 15, 15,

    //  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
        15, 14, 14, 14, 13, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10,

    //  32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        10, 10,  9,  9,  9,  8,  8,  8,  7,  7,  7,  7,  6,  6,  6,  5,

    //  48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63
         5,  5,  4,  4,  4,  4,  3,  3,  3,  2,  2,  2,  1,  1,  1,  1

    };

    static const uint64_t power_of_10[] = {
    //   0     1      2       3        4         5          6           7            8
        1UL, 10UL, 100UL, 1000UL, 10000UL, 100000UL, 1000000UL, 10000000UL, 100000000UL,
    //            9             10              11               12
        1000000000UL, 10000000000UL, 100000000000UL, 1000000000000UL,
    //               13                 14                  15                   16
        10000000000000UL, 100000000000000UL, 1000000000000000UL, 10000000000000000UL,
    //                   17                     18                      19
        100000000000000000UL, 1000000000000000000UL, 10000000000000000000UL
    };

    uint32_t y = clz_to_no_of_decimal_digits[count_leading_zeros(x)];
    if (x < power_of_10[y]) // Can be avoidable?
        y -= 1;
    return y;
}

/* Few useful links, and blog posts behind the inspiration.
 *
 * Andrei FB note, next link is his presentation
 * https://www.facebook.com/notes/facebook-engineering/three-optimization-tips-for-c/10151361643253920/
 * http://www.slideshare.net/andreialexandrescu1/three-optimization-tips-for-c-15708507
 *
 * Little deviation from the technique explained in the above blog post,
 *      Mulch two digits at a time using natual 'short int' access.
 *      Use bit hack along with lookup to find number of digits in decimal representation of input.
*/

FILE_SCOPE
// Useful if string data is packed into a 'Slice'
uint32_t fast_uint64_to_string(uint64_t val, char *p) {
    if (0 == val) {
        p[0] = '0';
        p[1] = '\0';

        return 1;
    }

    const uint32_t size = log10_characterstic(val);
    p[size+1] = '\0';

    p += size;
    while (val >= 10) {
        *(uint16_t *)(p - 1) = *(uint16_t *)(digits_in_ascii + 2 * (val % 100));
        val /= 100;
        p -= 2;
    }

    if (val)
        *p = '0' + val;

    return size;
}

FILE_SCOPE
std::string fast_uint64_to_string(uint64_t val) {
    if (0 == val)
        return std::move(std::string("0"));

    const uint32_t size = log10_characterstic(val);
    std::string s(size+1, '\0');
    char *p = const_cast<char *>(s.c_str()); // ugly hack, shut up the compiler

    p += size;
    while (val >= 10) {
        *(uint16_t *)(p - 1) = *(uint16_t *)(digits_in_ascii + 2 * (val % 100));
        val /= 100;
        p -= 2;
    }

    // Residue
    if (val)
        *p = '0' + val;

    return std::move(s);
}

#endif // _FAST_INT_TO_STRING_
