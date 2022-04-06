/* +++Date last modified: 05-Jul-1997 */

/*
**  BITCNTS.C - Test program for bit counting functions
**
**  public domain by Bob Stout & Auke Reitsma
*/

#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include <limits.h>
#include <time.h>
#include <float.h>
#include "common.h"
#include <scc/autoparallel.h>
#include "bitops.h"


//--------const for bitcnt2----------------------------
static char bits[256] =
{
      0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,  /* 0   - 15  */
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,  /* 16  - 31  */
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,  /* 32  - 47  */
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,  /* 48  - 63  */
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,  /* 64  - 79  */
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,  /* 80  - 95  */
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,  /* 96  - 111 */
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,  /* 112 - 127 */
      1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,  /* 128 - 143 */
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,  /* 144 - 159 */
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,  /* 160 - 175 */
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,  /* 176 - 191 */
      2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,  /* 192 - 207 */
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,  /* 208 - 223 */
      3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,  /* 224 - 239 */
      4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8   /* 240 - 255 */
};


//--------------------const end----------------------------


//bit count1
__attribute__((noinline, assertswarmified))
int bit_count(long x)
{
        int n = 0;
/*
** The loop will execute once for each bit of x set, this is in average
** twice as fast as the shift/test method.
*/
        if (x) do
              n++;
        while (0 != (x = x&(x-1))) ;
        return(n);
}
//-------------------------------------

__attribute__((noinline))
__attribute__((assertswarmified))
int bitcount(long i)
{
      i = ((i & 0xAAAAAAAAL) >>  1) + (i & 0x55555555L);
      i = ((i & 0xCCCCCCCCL) >>  2) + (i & 0x33333333L);
      i = ((i & 0xF0F0F0F0L) >>  4) + (i & 0x0F0F0F0FL);
      i = ((i & 0xFF00FF00L) >>  8) + (i & 0x00FF00FFL);
      i = ((i & 0xFFFF0000L) >> 16) + (i & 0x0000FFFFL);
      return (int)i;
}

//-------------------------------




//bitcount2
__attribute__((noinline))
__attribute__((assertswarmified))
int ntbl_bitcnt(long x)
{
      int cnt = bits[(int)(x & 0x0000000FL)];

      if (0L != (x >>= 4))
            cnt += ntbl_bitcnt(x);

      return cnt;
}

//-------------------------------------------
//bitcount3
__attribute__((noinline))
__attribute__((assertswarmified))
int ntbl_bitcount(long int x)
{
      return
            bits[ (int) (x & 0x0000000FUL)       ] +
            bits[ (int)((x & 0x000000F0UL) >> 4) ] +
            bits[ (int)((x & 0x00000F00UL) >> 8) ] +
            bits[ (int)((x & 0x0000F000UL) >> 12)] +
            bits[ (int)((x & 0x000F0000UL) >> 16)] +
            bits[ (int)((x & 0x00F00000UL) >> 20)] +
            bits[ (int)((x & 0x0F000000UL) >> 24)] +
            bits[ (int)((x & 0xF0000000UL) >> 28)];
}

/*
**  Count bits in each byte
**
**  by Bruce Wedding, works best on Watcom & Borland
*/

__attribute__((noinline))
__attribute__((assertswarmified))
int BW_btbl_bitcount(long int x)
{
      union 
      { 
            unsigned char ch[4]; 
            long y; 
      } U; 
 
      U.y = x; 
 
      return bits[ U.ch[0] ] + bits[ U.ch[1] ] + 
             bits[ U.ch[3] ] + bits[ U.ch[2] ]; 
}

/*
**  Count bits in each byte
**
**  by Auke Reitsma, works best on Microsoft, Symantec, and others
*/

__attribute__((noinline))
__attribute__((assertswarmified))
int AR_btbl_bitcount(long int x)
{
      unsigned char * Ptr = (unsigned char *) &x ;
      int Accu ;

      Accu  = bits[ *Ptr++ ];
      Accu += bits[ *Ptr++ ];
      Accu += bits[ *Ptr++ ];
      Accu += bits[ *Ptr ];
      return Accu;
}



//-------------------------------------------



/*
loop function
*/
//-------------------------------------------
__attribute__((noinline, assertswarmified))
void loop1(int iterations){
  clock_t start, stop;
  double ct;
  long seed;
  long n;
  long j;
  start = clock();
  for (j = n = 0, seed = rand(); j < iterations; j++, seed += 13){
      n += bit_count(seed);
    }
  stop = clock();
  ct = (stop - start) / (double)CLOCKS_PER_SEC;
  puts("Optimized 1 bit/loop counter");  
  printf("Time: %7.3f sec.; Bits: %ld\n", ct, n);
}

__attribute__((noinline, assertswarmified))
void loop2(int iterations){
  clock_t start, stop;
  double ct;
  long n;
  long j;
  long seed;
  start = clock();
  for (j = n = 0, seed = rand(); j < iterations; j++, seed += 13){
      n += bitcount(seed);
    }
  stop = clock();
  ct = (stop - start) / (double)CLOCKS_PER_SEC;
  puts("Ratko's mystery algorithm");  
  printf("Time: %7.3f sec.; Bits: %ld\n", ct, n);
}

__attribute__((noinline, assertswarmified))
void loop3(int iterations){
  clock_t start, stop;
  double ct;
  long n;
  long j;
  long seed;
  start = clock();
  for (j = n = 0, seed = rand(); j < iterations; j++, seed += 13){
      n += ntbl_bitcnt(seed);
    }
  stop = clock();
  ct = (stop - start) / (double)CLOCKS_PER_SEC;
  puts("Recursive bit count by nybbles");  
  printf("Time: %7.3f sec.; Bits: %ld\n", ct, n);
}

__attribute__((noinline, assertswarmified))
void loop4(int iterations){
  clock_t start, stop;
  double ct;
  long n;
  long j;
  long seed;
  start = clock();
  for (j = n = 0, seed = rand(); j < iterations; j++, seed += 13){
      n += ntbl_bitcount(seed);
    }
  stop = clock();
  ct = (stop - start) / (double)CLOCKS_PER_SEC;
  puts("Recursive bit count by nybbles");  
  printf("Time: %7.3f sec.; Bits: %ld\n", ct, n);
}

__attribute__((noinline, assertswarmified))
void loop5(int iterations){
  clock_t start, stop;
  double ct;
  long n;
  long j;
  long seed;
  start = clock();
  for (j = n = 0, seed = rand(); j < iterations; j++, seed += 13){
      n += BW_btbl_bitcount(seed);
    }
  stop = clock();
  ct = (stop - start) / (double)CLOCKS_PER_SEC;
  puts("Non-recursive bit count by bytes (BW)");  
  printf("Time: %7.3f sec.; Bits: %ld\n", ct, n);
}

__attribute__((noinline, assertswarmified))
void loop6(int iterations){
  clock_t start, stop;
  double ct;
  long n;
  long j;
  long seed;
  start = clock();
  for (j = n = 0, seed = rand(); j < iterations; j++, seed += 13){
      n += AR_btbl_bitcount(seed);
    }
  stop = clock();
  ct = (stop - start) / (double)CLOCKS_PER_SEC;
  puts("Non-recursive bit count by bytes (AR)");  
  printf("Time: %7.3f sec.; Bits: %ld\n", ct, n);
}


//--------------------------------------------------------

int main(int argc, char *argv[])
{
  int iterations;
 
  if (argc<2) {
    fprintf(stderr,"Usage: bitcnts <iterations>\n");
    exit(-1);
	}
  iterations=atoi(argv[1]);
  
  //loop1
  puts("Bit counter algorithm benchmark\n");
  SCC_PARALLEL({
      loop1(iterations);
      loop2(iterations);
      loop3(iterations);
      loop4(iterations);
      loop5(iterations);
      loop6(iterations);
    });
  return 0;
}
