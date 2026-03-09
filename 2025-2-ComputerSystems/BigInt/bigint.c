/* Guilherme Soares 3WC */
/* João Pedro 3WC */

#include <stdio.h>
#include "bigint.h"

#define NUM_BYTES (NUM_BITS/8)

/* 
Args: array of 16 unsigned chars (res), value to be assigned (val)
Return: assings the value to the array, but returns void
*/
void big_val (BigInt res, long val) {
    for (int i = 0; i < NUM_BYTES; i++) {
        res[i] = val & 0xFF;
        val = val >> 8;
    }

    return;
}

/* 
Args: BigInt for the answer (res), BigInt to be 2's complemented
Return: assings the 2's complement, but returns void
*/
void big_comp2(BigInt res, BigInt a) { // By definition: comp2(a) = ~a + 1
    int add = 1; // Boolean control variable for when adding 1 exceeds the 1 byte space (res[i] = 0xFF)
    for (int i = 0; i < NUM_BYTES; i++) {
        res[i] = ~a[i];
        if (add) {
            // If (res[i] + 1) would exceed one byte, keeps the control variable for the next byte
            if (res[i] == 0xFF) res[i] = 0x00;
            else {
                res[i] += 1;
                add = 0;
            }
        }
    }

    return;
}

/* 
Args: BigInt for the answer (res), two BigInts to add
Return: the sum of them in the BigInt res, but returns void
*/
void big_sum(BigInt res, BigInt a, BigInt b) {
    unsigned short s = 0; // Two bytes mask for the bits that exceeds 1 byte in the sum

    for (int i = 0; i < NUM_BYTES; i++) {
        s += a[i] + b[i];
        res[i] = s & 0xFF;
        s = s >> 8;
    }

    return;
}

/* 
Args: BigInt for the answer (res), two BigInts to subtract
Return: gives the subtratction in res, but returns void
*/
void big_sub(BigInt res, BigInt a, BigInt b) { // [a - b] = [a + (-b)]
    BigInt tmp; // Important because b would be messed in big_comp2(b,b) for the case big_sub(res, b, b)
    big_comp2(tmp, b);
    big_sum(res, a, tmp);

    return;
}

/* 
Args: BigInt for the answer (res), two BigInts to multiply
Return: gives the multiplication in res, but returns void
*/
void big_mul(BigInt res, BigInt a, BigInt b) { // Done the same way as a common multiplication learned at school
    big_val(res,0);

    unsigned short add, mul;
    // Multiply each "column" of a for each "column" of b
    for (int i = 0; i < NUM_BYTES; i++) {
        mul = add = 0;

        for (int j = 0; j < NUM_BYTES; j++) {
            if (i + j > 15) break; // Any out of bounds will be disconsidered

            mul += a[i] * b[j]; // Current multiplication is the sum of the carry in from the previous one with itself

            // The sum of the column (res[i + j]) will be the carry in from the previous sum (add) with the first byte of the current multiplication (mul)
            add += res[i + j] + (mul & 0xFF);
            res[i + j] = add & 0xFF; // The actual column is just the first byte of the sum (add)

            mul = mul >> 8;
            add = add >> 8;
        }
    }

    return;
}

/* 
Args: BigInt for the answer (res), BigInt to shift (a), number of bits to shift (n)
Return: gives "a" left shifted by n bits in res, but returns void
*/
void big_shl(BigInt res, BigInt a, int n) {
    int bytes = n / 8;
    int shift = n % 8;

    for (int i = 0; i < NUM_BYTES; i++) (i < bytes) ? (res[i] = 0) : (res[i] = a[i - bytes]);

    // If there are bits to be shifted inside a byte (and less than one byte)
    if (shift != 0) {
        char mask = 0x80;
        mask = mask >> (shift-1);
        unsigned char curr = 0, prev = 0; // curr = current byte mask, prev = previous byte mask

        // Bits that would exceed the byte are saved in the mask for the next byte
        for (int i = bytes; i < NUM_BYTES; i++) {
            prev = res[i] & mask;
            res[i] = res[i] << shift;
            res[i] |= curr;
            curr = prev >> (8 - shift);
        }
    }

    return;
}

/* 
Args: BigInt for the answer (res), BigInt to shift (a), number of bits to shift (n)
Return: gives "a" right shifted (logical) by n bits in res, but returns void
*/
void big_shr(BigInt res, BigInt a, int n) { // The same logic as shl
    int bytes = n / 8;
    int shift = n % 8;
    for (int i = 0; i < NUM_BYTES; i++) (i >= (NUM_BYTES - bytes)) ? (res[i] = 0) : (res[i] = a[i + bytes]);

    if (shift != 0) {
        unsigned char mask = 0x1;
        for (int i = 1; i < shift; i++) mask = (mask << 1) + 1;

        unsigned char curr = 0;
        unsigned char prev = 0;

        for (int i = NUM_BYTES - bytes-1; i >= 0; i--) {
            prev = (res[i] & mask);
            res[i] = res[i] >> shift;
            res[i] |= curr;
            curr = prev << (8 - shift);
        }
    }

    return;
}

/* 
Args: BigInt for the answer (res), BigInt to shift (a), number of bits to shift (n)
Return: gives "a" right shifted (arithmetical) by n bits in res, but returns void
*/
void big_sar(BigInt res, BigInt a, int n) {
    if ((a[NUM_BYTES - 1] | 0x7F) != 0xFF) big_shr(res, a, n); // If it's positive, just shifts right 
    else {    
        int bytes = n / 8;
        int shift = n % 8;

        big_shr(res, a, n);
        for (int i = NUM_BYTES - bytes; i <= NUM_BYTES - 1; i++) res[i] = 0xFF;

        if (shift != 0) {
            char mask = 0x80;
            mask = mask >> (shift-1);
            res[NUM_BYTES - bytes - 1] |= mask;
        }
    }

    return;
}