/* +++Date last modified: 05-Jul-1997 */

/*
**  Functions to maintain an arbitrary length array of bits
*/

#include "bitops.h"

__attribute__((noinline))
__attribute__((assertswarmified))
char *alloc_bit_array(size_t bits)
{
      char *set = calloc((bits + CHAR_BIT - 1) / CHAR_BIT, sizeof(char));

      return set;
}

__attribute__((noinline))
__attribute__((assertswarmified))
int getbit(char *set, int number)
{
        set += number / CHAR_BIT;
        return (*set & (1 << (number % CHAR_BIT))) != 0;    /* 0 or 1   */
}

__attribute__((noinline))
__attribute__((assertswarmified))
void setbit(char *set, int number, int value)
{
        set += number / CHAR_BIT;
        if (value)
                *set |= 1 << (number % CHAR_BIT);           /* set bit  */
        else    *set &= ~(1 << (number % CHAR_BIT));        /* clear bit*/
}

__attribute__((noinline))
__attribute__((assertswarmified))
void flipbit(char *set, int number)
{
        set += number / CHAR_BIT;
        *set ^= 1 << (number % CHAR_BIT);                   /* flip bit */
}
