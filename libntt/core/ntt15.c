#include "ntt-impl.h"

#define NC 18

static uint32_t 
ntt15_get_num_const(void)
{
  return NC;
}

extern void ntt3_init(spv_t out, sp_t p, sp_t d, 
			sp_t primroot, sp_t order);

extern void ntt5_init(spv_t out, sp_t p, sp_t d, 
			sp_t primroot, sp_t order);

void
ntt15_init(spv_t out, sp_t p, sp_t d, 
	  sp_t primroot, sp_t order)
{
  uint32_t i, j;
  uint32_t num3 = 3;
  uint32_t num5 = 6;
  spv_t root3 = (spv_t)alloca(num3 * sizeof(sp_t));
  spv_t root5 = (spv_t)alloca(num5 * sizeof(sp_t));

  ntt3_init(root3, p, d, primroot, order);
  ntt5_init(root5, p, d, primroot, order);

  for (i = 0; i < num3; i++)
    for (j = 0; j < num5; j++)
      out[num5 * i + j] = sp_mul(root3[i], root5[j], p, d);
}

static void
ntt15_run(spv_t x, spv_size_t stride,
	  sp_t p, spv_t ntt_const)
{
  sp_t a00, a01, a02, a03, a04, 
       a05, a06, a07, a08, a09, 
       a10, a11, a12, a13, a14;

  #ifdef HAVE_PARTIAL_MOD
  p *= 2;
  #endif

  {
    sp_t x00 = x[ 0 * stride];
    sp_t x01 = x[ 5 * stride];
    sp_t x02 = x[10 * stride];

    a01 = sp_add(x01, x02, p);
    a02 = sp_sub(x01, x02, p);

    a00 = sp_add(x00, a01, p);
  }
  {
    sp_t x03 = x[ 3 * stride];
    sp_t x04 = x[ 8 * stride];
    sp_t x05 = x[13 * stride];

    a04 = sp_add(x04, x05, p);
    a05 = sp_sub(x04, x05, p);

    a03 = sp_add(x03, a04, p);
  }
  {
    sp_t x08 = x[ 1 * stride];
    sp_t x06 = x[ 6 * stride];
    sp_t x07 = x[11 * stride];

    a07 = sp_add(x07, x08, p);
    a08 = sp_sub(x07, x08, p);

    a06 = sp_add(x06, a07, p);
  }
  {
    sp_t x11 = x[ 4 * stride];
    sp_t x09 = x[ 9 * stride];
    sp_t x10 = x[14 * stride];

    a10 = sp_add(x10, x11, p);
    a11 = sp_sub(x10, x11, p);

    a09 = sp_add(x09, a10, p);
  }
  {
    sp_t x13 = x[ 2 * stride];
    sp_t x14 = x[ 7 * stride];
    sp_t x12 = x[12 * stride];

    a13 = sp_add(x13, x14, p);
    a14 = sp_sub(x13, x14, p);

    a12 = sp_add(x12, a13, p);
  }
  {
    sp_t b0, b1, b2, b3, b4, b5;
    sp_t c1, c2, c3, c4;

    b0 = a00;
    b1 = a03;
    b4 = a06;
    b2 = a09;
    b3 = a12;

    c1 = sp_add(b1, b3, p);
    c3 = sp_sub(b1, b3, p);
    c2 = sp_add(b2, b4, p);
    c4 = sp_sub(b2, b4, p);

    b1 = sp_add(c1, c2, p);
    b2 = sp_sub_partial(c1, c2, p);
    b3 = c3;
    b4 = c4;
    b5 = sp_add_partial(c3, c4, p);

    b0 = sp_add(b0, b1, p);

    b1 = sp_ntt_mul(b1, ntt_const[1], ntt_const[NC+1], p);
    b2 = sp_ntt_mul(b2, ntt_const[2], ntt_const[NC+2], p);
    b3 = sp_ntt_mul(b3, ntt_const[3], ntt_const[NC+3], p);
    b4 = sp_ntt_mul(b4, ntt_const[4], ntt_const[NC+4], p);
    b5 = sp_ntt_mul(b5, ntt_const[5], ntt_const[NC+5], p);

    b1 = sp_add(b0, b1, p);

    c1 = sp_add(b1, b2, p);
    c2 = sp_sub(b1, b2, p);
    c3 = sp_add(b3, b5, p);
    c4 = sp_add(b4, b5, p);

    b1 = sp_add(c1, c3, p);
    b2 = sp_add(c2, c4, p);
    b3 = sp_sub(c1, c3, p);
    b4 = sp_sub(c2, c4, p);

    a00 = b0;
    a03 = b4;
    a06 = b3;
    a09 = b1;
    a12 = b2;
  }
  {
    sp_t b0, b1, b2, b3, b4, b5;
    sp_t c1, c2, c3, c4;

    b0 = a01;
    b1 = a04;
    b4 = a07;
    b2 = a10;
    b3 = a13;

    c1 = sp_add(b1, b3, p);
    c3 = sp_sub(b1, b3, p);
    c2 = sp_add(b2, b4, p);
    c4 = sp_sub(b2, b4, p);

    b1 = sp_add(c1, c2, p);
    b2 = sp_sub_partial(c1, c2, p);
    b3 = c3;
    b4 = c4;
    b5 = sp_add_partial(c3, c4, p);

    b0 = sp_add_partial(b0, b1, p);

    b0 = sp_ntt_mul(b0, ntt_const[6], ntt_const[NC+6], p);
    b1 = sp_ntt_mul(b1, ntt_const[7], ntt_const[NC+7], p);
    b2 = sp_ntt_mul(b2, ntt_const[8], ntt_const[NC+8], p);
    b3 = sp_ntt_mul(b3, ntt_const[9], ntt_const[NC+9], p);
    b4 = sp_ntt_mul(b4, ntt_const[10], ntt_const[NC+10], p);
    b5 = sp_ntt_mul(b5, ntt_const[11], ntt_const[NC+11], p);

    b1 = sp_add(b0, b1, p);

    c1 = sp_add(b1, b2, p);
    c2 = sp_sub(b1, b2, p);
    c3 = sp_add(b3, b5, p);
    c4 = sp_add(b4, b5, p);

    b1 = sp_add(c1, c3, p);
    b2 = sp_add(c2, c4, p);
    b3 = sp_sub(c1, c3, p);
    b4 = sp_sub(c2, c4, p);

    a01 = b0;
    a04 = b4;
    a07 = b3;
    a10 = b1;
    a13 = b2;
  }

  {
    sp_t b0, b1, b2, b3, b4, b5;
    sp_t c1, c2, c3, c4;

    b0 = a02;
    b1 = a05;
    b4 = a08;
    b2 = a11;
    b3 = a14;

    c1 = sp_add(b1, b3, p);
    c3 = sp_sub(b1, b3, p);
    c2 = sp_add(b2, b4, p);
    c4 = sp_sub(b2, b4, p);

    b1 = sp_add(c1, c2, p);
    b2 = sp_sub_partial(c1, c2, p);
    b3 = c3;
    b4 = c4;
    b5 = sp_add_partial(c3, c4, p);

    b0 = sp_add_partial(b0, b1, p);

    b0 = sp_ntt_mul(b0, ntt_const[12], ntt_const[NC+12], p);
    b1 = sp_ntt_mul(b1, ntt_const[13], ntt_const[NC+13], p);
    b2 = sp_ntt_mul(b2, ntt_const[14], ntt_const[NC+14], p);
    b3 = sp_ntt_mul(b3, ntt_const[15], ntt_const[NC+15], p);
    b4 = sp_ntt_mul(b4, ntt_const[16], ntt_const[NC+16], p);
    b5 = sp_ntt_mul(b5, ntt_const[17], ntt_const[NC+17], p);

    b1 = sp_add(b0, b1, p);

    c1 = sp_add(b1, b2, p);
    c2 = sp_sub(b1, b2, p);
    c3 = sp_add(b3, b5, p);
    c4 = sp_add(b4, b5, p);

    b1 = sp_add(c1, c3, p);
    b2 = sp_add(c2, c4, p);
    b3 = sp_sub(c1, c3, p);
    b4 = sp_sub(c2, c4, p);

    a02 = b0;
    a05 = b4;
    a08 = b3;
    a11 = b1;
    a14 = b2;
  }
  {
    sp_t x00, x01, x02;

    a01 = sp_add(a00, a01, p);

    x00 = a00;
    x01 = sp_add(a01, a02, p);
    x02 = sp_sub(a01, a02, p);

    x[ 0 * stride] = x00;
    x[ 5 * stride] = x02;
    x[10 * stride] = x01;
  }
  {
    sp_t x03, x04, x05;

    a04 = sp_add(a03, a04, p);

    x03 = a03;
    x04 = sp_add(a04, a05, p);
    x05 = sp_sub(a04, a05, p);

    x[ 1 * stride] = x04;
    x[ 6 * stride] = x03;
    x[11 * stride] = x05;
  }
  {
    sp_t x06, x07, x08;

    a07 = sp_add(a06, a07, p);

    x06 = a06;
    x07 = sp_add(a07, a08, p);
    x08 = sp_sub(a07, a08, p);

    x[ 2 * stride] = x08;
    x[ 7 * stride] = x07;
    x[12 * stride] = x06;
  }
  {
    sp_t x09, x10, x11;

    a10 = sp_add(a09, a10, p);

    x09 = a09;
    x10 = sp_add(a10, a11, p);
    x11 = sp_sub(a10, a11, p);

    x[ 3 * stride] = x09;
    x[ 8 * stride] = x11;
    x[13 * stride] = x10;
  }
  {
    sp_t x12, x13, x14;

    a13 = sp_add(a12, a13, p);

    x12 = a12;
    x13 = sp_add(a13, a14, p);
    x14 = sp_sub(a13, a14, p);

    x[ 4 * stride] = x13;
    x[ 9 * stride] = x12;
    x[14 * stride] = x14;
  }

}

#ifdef HAVE_SSE2
static void
ntt15_run_simd(spv_t x, spv_size_t stride,
	  sp_t p, spv_t ntt_const)
{
  sp_simd_t a00, a01, a02, a03, a04, 
            a05, a06, a07, a08, a09, 
            a10, a11, a12, a13, a14;

  #ifdef HAVE_PARTIAL_MOD
  p *= 2;
  #endif

  {
    sp_simd_t x00 = sp_simd_gather(x +  0 * stride);
    sp_simd_t x01 = sp_simd_gather(x +  5 * stride);
    sp_simd_t x02 = sp_simd_gather(x + 10 * stride);

    a01 = sp_simd_add(x01, x02, p);
    a02 = sp_simd_sub(x01, x02, p);

    a00 = sp_simd_add(x00, a01, p);
  }
  {
    sp_simd_t x03 = sp_simd_gather(x +  3 * stride);
    sp_simd_t x04 = sp_simd_gather(x +  8 * stride);
    sp_simd_t x05 = sp_simd_gather(x + 13 * stride);

    a04 = sp_simd_add(x04, x05, p);
    a05 = sp_simd_sub(x04, x05, p);

    a03 = sp_simd_add(x03, a04, p);
  }
  {
    sp_simd_t x08 = sp_simd_gather(x +  1 * stride);
    sp_simd_t x06 = sp_simd_gather(x +  6 * stride);
    sp_simd_t x07 = sp_simd_gather(x + 11 * stride);

    a07 = sp_simd_add(x07, x08, p);
    a08 = sp_simd_sub(x07, x08, p);

    a06 = sp_simd_add(x06, a07, p);
  }
  {
    sp_simd_t x11 = sp_simd_gather(x +  4 * stride);
    sp_simd_t x09 = sp_simd_gather(x +  9 * stride);
    sp_simd_t x10 = sp_simd_gather(x + 14 * stride);

    a10 = sp_simd_add(x10, x11, p);
    a11 = sp_simd_sub(x10, x11, p);

    a09 = sp_simd_add(x09, a10, p);
  }
  {
    sp_simd_t x13 = sp_simd_gather(x +  2 * stride);
    sp_simd_t x14 = sp_simd_gather(x +  7 * stride);
    sp_simd_t x12 = sp_simd_gather(x + 12 * stride);

    a13 = sp_simd_add(x13, x14, p);
    a14 = sp_simd_sub(x13, x14, p);

    a12 = sp_simd_add(x12, a13, p);
  }
  {
    sp_simd_t b0, b1, b2, b3, b4, b5;
    sp_simd_t c1, c2, c3, c4;

    b0 = a00;
    b1 = a03;
    b4 = a06;
    b2 = a09;
    b3 = a12;

    c1 = sp_simd_add(b1, b3, p);
    c3 = sp_simd_sub(b1, b3, p);
    c2 = sp_simd_add(b2, b4, p);
    c4 = sp_simd_sub(b2, b4, p);

    b1 = sp_simd_add(c1, c2, p);
    b2 = sp_simd_sub_partial(c1, c2, p);
    b3 = c3;
    b4 = c4;
    b5 = sp_simd_add_partial(c3, c4, p);

    b0 = sp_simd_add(b0, b1, p);

    b1 = sp_simd_ntt_mul(b1, ntt_const[1], ntt_const[NC+1], p);
    b2 = sp_simd_ntt_mul(b2, ntt_const[2], ntt_const[NC+2], p);
    b3 = sp_simd_ntt_mul(b3, ntt_const[3], ntt_const[NC+3], p);
    b4 = sp_simd_ntt_mul(b4, ntt_const[4], ntt_const[NC+4], p);
    b5 = sp_simd_ntt_mul(b5, ntt_const[5], ntt_const[NC+5], p);

    b1 = sp_simd_add(b0, b1, p);

    c1 = sp_simd_add(b1, b2, p);
    c2 = sp_simd_sub(b1, b2, p);
    c3 = sp_simd_add(b3, b5, p);
    c4 = sp_simd_add(b4, b5, p);

    b1 = sp_simd_add(c1, c3, p);
    b2 = sp_simd_add(c2, c4, p);
    b3 = sp_simd_sub(c1, c3, p);
    b4 = sp_simd_sub(c2, c4, p);

    a00 = b0;
    a03 = b4;
    a06 = b3;
    a09 = b1;
    a12 = b2;
  }
  {
    sp_simd_t b0, b1, b2, b3, b4, b5;
    sp_simd_t c1, c2, c3, c4;

    b0 = a01;
    b1 = a04;
    b4 = a07;
    b2 = a10;
    b3 = a13;

    c1 = sp_simd_add(b1, b3, p);
    c3 = sp_simd_sub(b1, b3, p);
    c2 = sp_simd_add(b2, b4, p);
    c4 = sp_simd_sub(b2, b4, p);

    b1 = sp_simd_add(c1, c2, p);
    b2 = sp_simd_sub_partial(c1, c2, p);
    b3 = c3;
    b4 = c4;
    b5 = sp_simd_add_partial(c3, c4, p);

    b0 = sp_simd_add_partial(b0, b1, p);

    b0 = sp_simd_ntt_mul(b0, ntt_const[6], ntt_const[NC+6], p);
    b1 = sp_simd_ntt_mul(b1, ntt_const[7], ntt_const[NC+7], p);
    b2 = sp_simd_ntt_mul(b2, ntt_const[8], ntt_const[NC+8], p);
    b3 = sp_simd_ntt_mul(b3, ntt_const[9], ntt_const[NC+9], p);
    b4 = sp_simd_ntt_mul(b4, ntt_const[10], ntt_const[NC+10], p);
    b5 = sp_simd_ntt_mul(b5, ntt_const[11], ntt_const[NC+11], p);

    b1 = sp_simd_add(b0, b1, p);

    c1 = sp_simd_add(b1, b2, p);
    c2 = sp_simd_sub(b1, b2, p);
    c3 = sp_simd_add(b3, b5, p);
    c4 = sp_simd_add(b4, b5, p);

    b1 = sp_simd_add(c1, c3, p);
    b2 = sp_simd_add(c2, c4, p);
    b3 = sp_simd_sub(c1, c3, p);
    b4 = sp_simd_sub(c2, c4, p);

    a01 = b0;
    a04 = b4;
    a07 = b3;
    a10 = b1;
    a13 = b2;
  }

  {
    sp_simd_t b0, b1, b2, b3, b4, b5;
    sp_simd_t c1, c2, c3, c4;

    b0 = a02;
    b1 = a05;
    b4 = a08;
    b2 = a11;
    b3 = a14;

    c1 = sp_simd_add(b1, b3, p);
    c3 = sp_simd_sub(b1, b3, p);
    c2 = sp_simd_add(b2, b4, p);
    c4 = sp_simd_sub(b2, b4, p);

    b1 = sp_simd_add(c1, c2, p);
    b2 = sp_simd_sub_partial(c1, c2, p);
    b3 = c3;
    b4 = c4;
    b5 = sp_simd_add_partial(c3, c4, p);

    b0 = sp_simd_add_partial(b0, b1, p);

    b0 = sp_simd_ntt_mul(b0, ntt_const[12], ntt_const[NC+12], p);
    b1 = sp_simd_ntt_mul(b1, ntt_const[13], ntt_const[NC+13], p);
    b2 = sp_simd_ntt_mul(b2, ntt_const[14], ntt_const[NC+14], p);
    b3 = sp_simd_ntt_mul(b3, ntt_const[15], ntt_const[NC+15], p);
    b4 = sp_simd_ntt_mul(b4, ntt_const[16], ntt_const[NC+16], p);
    b5 = sp_simd_ntt_mul(b5, ntt_const[17], ntt_const[NC+17], p);

    b1 = sp_simd_add(b0, b1, p);

    c1 = sp_simd_add(b1, b2, p);
    c2 = sp_simd_sub(b1, b2, p);
    c3 = sp_simd_add(b3, b5, p);
    c4 = sp_simd_add(b4, b5, p);

    b1 = sp_simd_add(c1, c3, p);
    b2 = sp_simd_add(c2, c4, p);
    b3 = sp_simd_sub(c1, c3, p);
    b4 = sp_simd_sub(c2, c4, p);

    a02 = b0;
    a05 = b4;
    a08 = b3;
    a11 = b1;
    a14 = b2;
  }
  {
    sp_simd_t x00, x01, x02;

    a01 = sp_simd_add(a00, a01, p);

    x00 = a00;
    x01 = sp_simd_add(a01, a02, p);
    x02 = sp_simd_sub(a01, a02, p);

    sp_simd_scatter(x00, x +  0 * stride);
    sp_simd_scatter(x02, x +  5 * stride);
    sp_simd_scatter(x01, x + 10 * stride);
  }
  {
    sp_simd_t x03, x04, x05;

    a04 = sp_simd_add(a03, a04, p);

    x03 = a03;
    x04 = sp_simd_add(a04, a05, p);
    x05 = sp_simd_sub(a04, a05, p);

    sp_simd_scatter(x04, x +  1 * stride);
    sp_simd_scatter(x03, x +  6 * stride);
    sp_simd_scatter(x05, x + 11 * stride);
  }
  {
    sp_simd_t x06, x07, x08;

    a07 = sp_simd_add(a06, a07, p);

    x06 = a06;
    x07 = sp_simd_add(a07, a08, p);
    x08 = sp_simd_sub(a07, a08, p);

    sp_simd_scatter(x08, x +  2 * stride);
    sp_simd_scatter(x07, x +  7 * stride);
    sp_simd_scatter(x06, x + 12 * stride);
  }
  {
    sp_simd_t x09, x10, x11;

    a10 = sp_simd_add(a09, a10, p);

    x09 = a09;
    x10 = sp_simd_add(a10, a11, p);
    x11 = sp_simd_sub(a10, a11, p);

    sp_simd_scatter(x09, x +  3 * stride);
    sp_simd_scatter(x11, x +  8 * stride);
    sp_simd_scatter(x10, x + 13 * stride);
  }
  {
    sp_simd_t x12, x13, x14;

    a13 = sp_simd_add(a12, a13, p);

    x12 = a12;
    x13 = sp_simd_add(a13, a14, p);
    x14 = sp_simd_sub(a13, a14, p);

    sp_simd_scatter(x13, x +  4 * stride);
    sp_simd_scatter(x12, x +  9 * stride);
    sp_simd_scatter(x14, x + 14 * stride);
  }
}
#endif

static void
ntt15_twiddle_run(spv_t x, spv_size_t stride,
	  spv_size_t num_transforms,
	  sp_t p, spv_t ntt_const)
{
  spv_size_t i = 0;

#ifdef HAVE_SSE2
  spv_size_t num_simd = SP_SIMD_VSIZE * (num_transforms / SP_SIMD_VSIZE);

  for (i = 0; i < num_simd; i += SP_SIMD_VSIZE)
      ntt15_run_simd(x + i, stride, p, ntt_const);
#endif

  for (; i < num_transforms; i++)
    ntt15_run(x + i, stride, p, ntt_const);
}

static void
ntt15_pfa_run_core(spv_t x, spv_size_t start,
	  spv_size_t inc, spv_size_t n,
	  sp_t p, spv_t ntt_const)
{
  spv_size_t j00, j01, j02, j03, j04,
             j05, j06, j07, j08, j09,
	     j10, j11, j12, j13, j14;

  sp_t a00, a01, a02, a03, a04, 
       a05, a06, a07, a08, a09, 
       a10, a11, a12, a13, a14;

  #ifdef HAVE_PARTIAL_MOD
  p *= 2;
  #endif

  j00 = start;
  j01 = sp_array_inc(j00, inc, n);
  j02 = sp_array_inc(j00, 2 * inc, n);
  j03 = sp_array_inc(j00, 3 * inc, n);
  j04 = sp_array_inc(j00, 4 * inc, n);
  j05 = sp_array_inc(j00, 5 * inc, n);
  j06 = sp_array_inc(j00, 6 * inc, n);
  j07 = sp_array_inc(j00, 7 * inc, n);
  j08 = sp_array_inc(j00, 8 * inc, n);
  j09 = sp_array_inc(j00, 9 * inc, n);
  j10 = sp_array_inc(j00, 10 * inc, n);
  j11 = sp_array_inc(j00, 11 * inc, n);
  j12 = sp_array_inc(j00, 12 * inc, n);
  j13 = sp_array_inc(j00, 13 * inc, n);
  j14 = sp_array_inc(j00, 14 * inc, n);

  {
    sp_t x00 = x[j00];
    sp_t x01 = x[j05];
    sp_t x02 = x[j10];

    a01 = sp_add(x01, x02, p);
    a02 = sp_sub(x01, x02, p);

    a00 = sp_add(x00, a01, p);
  }
  {
    sp_t x03 = x[j03];
    sp_t x04 = x[j08];
    sp_t x05 = x[j13];

    a04 = sp_add(x04, x05, p);
    a05 = sp_sub(x04, x05, p);

    a03 = sp_add(x03, a04, p);
  }
  {
    sp_t x08 = x[j01];
    sp_t x06 = x[j06];
    sp_t x07 = x[j11];

    a07 = sp_add(x07, x08, p);
    a08 = sp_sub(x07, x08, p);

    a06 = sp_add(x06, a07, p);
  }
  {
    sp_t x11 = x[j04];
    sp_t x09 = x[j09];
    sp_t x10 = x[j14];

    a10 = sp_add(x10, x11, p);
    a11 = sp_sub(x10, x11, p);

    a09 = sp_add(x09, a10, p);
  }
  {
    sp_t x13 = x[j02];
    sp_t x14 = x[j07];
    sp_t x12 = x[j12];

    a13 = sp_add(x13, x14, p);
    a14 = sp_sub(x13, x14, p);

    a12 = sp_add(x12, a13, p);
  }
  {
    sp_t b0, b1, b2, b3, b4, b5;
    sp_t c1, c2, c3, c4;

    b0 = a00;
    b1 = a03;
    b4 = a06;
    b2 = a09;
    b3 = a12;

    c1 = sp_add(b1, b3, p);
    c3 = sp_sub(b1, b3, p);
    c2 = sp_add(b2, b4, p);
    c4 = sp_sub(b2, b4, p);

    b1 = sp_add(c1, c2, p);
    b2 = sp_sub_partial(c1, c2, p);
    b3 = c3;
    b4 = c4;
    b5 = sp_add_partial(c3, c4, p);

    b0 = sp_add(b0, b1, p);

    b1 = sp_ntt_mul(b1, ntt_const[1], ntt_const[NC+1], p);
    b2 = sp_ntt_mul(b2, ntt_const[2], ntt_const[NC+2], p);
    b3 = sp_ntt_mul(b3, ntt_const[3], ntt_const[NC+3], p);
    b4 = sp_ntt_mul(b4, ntt_const[4], ntt_const[NC+4], p);
    b5 = sp_ntt_mul(b5, ntt_const[5], ntt_const[NC+5], p);

    b1 = sp_add(b0, b1, p);

    c1 = sp_add(b1, b2, p);
    c2 = sp_sub(b1, b2, p);
    c3 = sp_add(b3, b5, p);
    c4 = sp_add(b4, b5, p);

    b1 = sp_add(c1, c3, p);
    b2 = sp_add(c2, c4, p);
    b3 = sp_sub(c1, c3, p);
    b4 = sp_sub(c2, c4, p);

    a00 = b0;
    a03 = b4;
    a06 = b3;
    a09 = b1;
    a12 = b2;
  }
  {
    sp_t b0, b1, b2, b3, b4, b5;
    sp_t c1, c2, c3, c4;

    b0 = a01;
    b1 = a04;
    b4 = a07;
    b2 = a10;
    b3 = a13;

    c1 = sp_add(b1, b3, p);
    c3 = sp_sub(b1, b3, p);
    c2 = sp_add(b2, b4, p);
    c4 = sp_sub(b2, b4, p);

    b1 = sp_add(c1, c2, p);
    b2 = sp_sub_partial(c1, c2, p);
    b3 = c3;
    b4 = c4;
    b5 = sp_add_partial(c3, c4, p);

    b0 = sp_add_partial(b0, b1, p);

    b0 = sp_ntt_mul(b0, ntt_const[6], ntt_const[NC+6], p);
    b1 = sp_ntt_mul(b1, ntt_const[7], ntt_const[NC+7], p);
    b2 = sp_ntt_mul(b2, ntt_const[8], ntt_const[NC+8], p);
    b3 = sp_ntt_mul(b3, ntt_const[9], ntt_const[NC+9], p);
    b4 = sp_ntt_mul(b4, ntt_const[10], ntt_const[NC+10], p);
    b5 = sp_ntt_mul(b5, ntt_const[11], ntt_const[NC+11], p);

    b1 = sp_add(b0, b1, p);

    c1 = sp_add(b1, b2, p);
    c2 = sp_sub(b1, b2, p);
    c3 = sp_add(b3, b5, p);
    c4 = sp_add(b4, b5, p);

    b1 = sp_add(c1, c3, p);
    b2 = sp_add(c2, c4, p);
    b3 = sp_sub(c1, c3, p);
    b4 = sp_sub(c2, c4, p);

    a01 = b0;
    a04 = b4;
    a07 = b3;
    a10 = b1;
    a13 = b2;
  }

  {
    sp_t b0, b1, b2, b3, b4, b5;
    sp_t c1, c2, c3, c4;

    b0 = a02;
    b1 = a05;
    b4 = a08;
    b2 = a11;
    b3 = a14;

    c1 = sp_add(b1, b3, p);
    c3 = sp_sub(b1, b3, p);
    c2 = sp_add(b2, b4, p);
    c4 = sp_sub(b2, b4, p);

    b1 = sp_add(c1, c2, p);
    b2 = sp_sub_partial(c1, c2, p);
    b3 = c3;
    b4 = c4;
    b5 = sp_add_partial(c3, c4, p);

    b0 = sp_add_partial(b0, b1, p);

    b0 = sp_ntt_mul(b0, ntt_const[12], ntt_const[NC+12], p);
    b1 = sp_ntt_mul(b1, ntt_const[13], ntt_const[NC+13], p);
    b2 = sp_ntt_mul(b2, ntt_const[14], ntt_const[NC+14], p);
    b3 = sp_ntt_mul(b3, ntt_const[15], ntt_const[NC+15], p);
    b4 = sp_ntt_mul(b4, ntt_const[16], ntt_const[NC+16], p);
    b5 = sp_ntt_mul(b5, ntt_const[17], ntt_const[NC+17], p);

    b1 = sp_add(b0, b1, p);

    c1 = sp_add(b1, b2, p);
    c2 = sp_sub(b1, b2, p);
    c3 = sp_add(b3, b5, p);
    c4 = sp_add(b4, b5, p);

    b1 = sp_add(c1, c3, p);
    b2 = sp_add(c2, c4, p);
    b3 = sp_sub(c1, c3, p);
    b4 = sp_sub(c2, c4, p);

    a02 = b0;
    a05 = b4;
    a08 = b3;
    a11 = b1;
    a14 = b2;
  }
  {
    sp_t x00, x01, x02;

    a01 = sp_add(a00, a01, p);

    x00 = a00;
    x01 = sp_add(a01, a02, p);
    x02 = sp_sub(a01, a02, p);

    x[j00] = x00;
    x[j05] = x02;
    x[j10] = x01;
  }
  {
    sp_t x03, x04, x05;

    a04 = sp_add(a03, a04, p);

    x03 = a03;
    x04 = sp_add(a04, a05, p);
    x05 = sp_sub(a04, a05, p);

    x[j01] = x04;
    x[j06] = x03;
    x[j11] = x05;
  }
  {
    sp_t x06, x07, x08;

    a07 = sp_add(a06, a07, p);

    x06 = a06;
    x07 = sp_add(a07, a08, p);
    x08 = sp_sub(a07, a08, p);

    x[j02] = x08;
    x[j07] = x07;
    x[j12] = x06;
  }
  {
    sp_t x09, x10, x11;

    a10 = sp_add(a09, a10, p);

    x09 = a09;
    x10 = sp_add(a10, a11, p);
    x11 = sp_sub(a10, a11, p);

    x[j03] = x09;
    x[j08] = x11;
    x[j13] = x10;
  }
  {
    sp_t x12, x13, x14;

    a13 = sp_add(a12, a13, p);

    x12 = a12;
    x13 = sp_add(a13, a14, p);
    x14 = sp_sub(a13, a14, p);

    x[j04] = x13;
    x[j09] = x12;
    x[j14] = x14;
  }
}

#ifdef HAVE_SSE2
static void
ntt15_pfa_run_core_simd(spv_t x, spv_size_t start,
	  spv_size_t inc, spv_size_t inc2, spv_size_t n,
	  sp_t p, spv_t ntt_const)
{
  spv_size_t j00, j01, j02, j03, j04,
             j05, j06, j07, j08, j09,
	     j10, j11, j12, j13, j14;

  sp_simd_t a00, a01, a02, a03, a04, 
            a05, a06, a07, a08, a09, 
            a10, a11, a12, a13, a14;

  #ifdef HAVE_PARTIAL_MOD
  p *= 2;
  #endif

  j00 = start;
  j01 = sp_array_inc(j00, inc, n);
  j02 = sp_array_inc(j00, 2 * inc, n);
  j03 = sp_array_inc(j00, 3 * inc, n);
  j04 = sp_array_inc(j00, 4 * inc, n);
  j05 = sp_array_inc(j00, 5 * inc, n);
  j06 = sp_array_inc(j00, 6 * inc, n);
  j07 = sp_array_inc(j00, 7 * inc, n);
  j08 = sp_array_inc(j00, 8 * inc, n);
  j09 = sp_array_inc(j00, 9 * inc, n);
  j10 = sp_array_inc(j00, 10 * inc, n);
  j11 = sp_array_inc(j00, 11 * inc, n);
  j12 = sp_array_inc(j00, 12 * inc, n);
  j13 = sp_array_inc(j00, 13 * inc, n);
  j14 = sp_array_inc(j00, 14 * inc, n);

  {
    sp_simd_t x00 = sp_simd_pfa_gather(x, j00, inc2, n);
    sp_simd_t x01 = sp_simd_pfa_gather(x, j05, inc2, n);
    sp_simd_t x02 = sp_simd_pfa_gather(x, j10, inc2, n);

    a01 = sp_simd_add(x01, x02, p);
    a02 = sp_simd_sub(x01, x02, p);

    a00 = sp_simd_add(x00, a01, p);
  }
  {
    sp_simd_t x03 = sp_simd_pfa_gather(x, j03, inc2, n);
    sp_simd_t x04 = sp_simd_pfa_gather(x, j08, inc2, n);
    sp_simd_t x05 = sp_simd_pfa_gather(x, j13, inc2, n);

    a04 = sp_simd_add(x04, x05, p);
    a05 = sp_simd_sub(x04, x05, p);

    a03 = sp_simd_add(x03, a04, p);
  }
  {
    sp_simd_t x08 = sp_simd_pfa_gather(x, j01, inc2, n);
    sp_simd_t x06 = sp_simd_pfa_gather(x, j06, inc2, n);
    sp_simd_t x07 = sp_simd_pfa_gather(x, j11, inc2, n);

    a07 = sp_simd_add(x07, x08, p);
    a08 = sp_simd_sub(x07, x08, p);

    a06 = sp_simd_add(x06, a07, p);
  }
  {
    sp_simd_t x11 = sp_simd_pfa_gather(x, j04, inc2, n);
    sp_simd_t x09 = sp_simd_pfa_gather(x, j09, inc2, n);
    sp_simd_t x10 = sp_simd_pfa_gather(x, j14, inc2, n);

    a10 = sp_simd_add(x10, x11, p);
    a11 = sp_simd_sub(x10, x11, p);

    a09 = sp_simd_add(x09, a10, p);
  }
  {
    sp_simd_t x13 = sp_simd_pfa_gather(x, j02, inc2, n);
    sp_simd_t x14 = sp_simd_pfa_gather(x, j07, inc2, n);
    sp_simd_t x12 = sp_simd_pfa_gather(x, j12, inc2, n);

    a13 = sp_simd_add(x13, x14, p);
    a14 = sp_simd_sub(x13, x14, p);

    a12 = sp_simd_add(x12, a13, p);
  }
  {
    sp_simd_t b0, b1, b2, b3, b4, b5;
    sp_simd_t c1, c2, c3, c4;

    b0 = a00;
    b1 = a03;
    b4 = a06;
    b2 = a09;
    b3 = a12;

    c1 = sp_simd_add(b1, b3, p);
    c3 = sp_simd_sub(b1, b3, p);
    c2 = sp_simd_add(b2, b4, p);
    c4 = sp_simd_sub(b2, b4, p);

    b1 = sp_simd_add(c1, c2, p);
    b2 = sp_simd_sub_partial(c1, c2, p);
    b3 = c3;
    b4 = c4;
    b5 = sp_simd_add_partial(c3, c4, p);

    b0 = sp_simd_add(b0, b1, p);

    b1 = sp_simd_ntt_mul(b1, ntt_const[1], ntt_const[NC+1], p);
    b2 = sp_simd_ntt_mul(b2, ntt_const[2], ntt_const[NC+2], p);
    b3 = sp_simd_ntt_mul(b3, ntt_const[3], ntt_const[NC+3], p);
    b4 = sp_simd_ntt_mul(b4, ntt_const[4], ntt_const[NC+4], p);
    b5 = sp_simd_ntt_mul(b5, ntt_const[5], ntt_const[NC+5], p);

    b1 = sp_simd_add(b0, b1, p);

    c1 = sp_simd_add(b1, b2, p);
    c2 = sp_simd_sub(b1, b2, p);
    c3 = sp_simd_add(b3, b5, p);
    c4 = sp_simd_add(b4, b5, p);

    b1 = sp_simd_add(c1, c3, p);
    b2 = sp_simd_add(c2, c4, p);
    b3 = sp_simd_sub(c1, c3, p);
    b4 = sp_simd_sub(c2, c4, p);

    a00 = b0;
    a03 = b4;
    a06 = b3;
    a09 = b1;
    a12 = b2;
  }
  {
    sp_simd_t b0, b1, b2, b3, b4, b5;
    sp_simd_t c1, c2, c3, c4;

    b0 = a01;
    b1 = a04;
    b4 = a07;
    b2 = a10;
    b3 = a13;

    c1 = sp_simd_add(b1, b3, p);
    c3 = sp_simd_sub(b1, b3, p);
    c2 = sp_simd_add(b2, b4, p);
    c4 = sp_simd_sub(b2, b4, p);

    b1 = sp_simd_add(c1, c2, p);
    b2 = sp_simd_sub_partial(c1, c2, p);
    b3 = c3;
    b4 = c4;
    b5 = sp_simd_add_partial(c3, c4, p);

    b0 = sp_simd_add_partial(b0, b1, p);

    b0 = sp_simd_ntt_mul(b0, ntt_const[6], ntt_const[NC+6], p);
    b1 = sp_simd_ntt_mul(b1, ntt_const[7], ntt_const[NC+7], p);
    b2 = sp_simd_ntt_mul(b2, ntt_const[8], ntt_const[NC+8], p);
    b3 = sp_simd_ntt_mul(b3, ntt_const[9], ntt_const[NC+9], p);
    b4 = sp_simd_ntt_mul(b4, ntt_const[10], ntt_const[NC+10], p);
    b5 = sp_simd_ntt_mul(b5, ntt_const[11], ntt_const[NC+11], p);

    b1 = sp_simd_add(b0, b1, p);

    c1 = sp_simd_add(b1, b2, p);
    c2 = sp_simd_sub(b1, b2, p);
    c3 = sp_simd_add(b3, b5, p);
    c4 = sp_simd_add(b4, b5, p);

    b1 = sp_simd_add(c1, c3, p);
    b2 = sp_simd_add(c2, c4, p);
    b3 = sp_simd_sub(c1, c3, p);
    b4 = sp_simd_sub(c2, c4, p);

    a01 = b0;
    a04 = b4;
    a07 = b3;
    a10 = b1;
    a13 = b2;
  }

  {
    sp_simd_t b0, b1, b2, b3, b4, b5;
    sp_simd_t c1, c2, c3, c4;

    b0 = a02;
    b1 = a05;
    b4 = a08;
    b2 = a11;
    b3 = a14;

    c1 = sp_simd_add(b1, b3, p);
    c3 = sp_simd_sub(b1, b3, p);
    c2 = sp_simd_add(b2, b4, p);
    c4 = sp_simd_sub(b2, b4, p);

    b1 = sp_simd_add(c1, c2, p);
    b2 = sp_simd_sub_partial(c1, c2, p);
    b3 = c3;
    b4 = c4;
    b5 = sp_simd_add_partial(c3, c4, p);

    b0 = sp_simd_add_partial(b0, b1, p);

    b0 = sp_simd_ntt_mul(b0, ntt_const[12], ntt_const[NC+12], p);
    b1 = sp_simd_ntt_mul(b1, ntt_const[13], ntt_const[NC+13], p);
    b2 = sp_simd_ntt_mul(b2, ntt_const[14], ntt_const[NC+14], p);
    b3 = sp_simd_ntt_mul(b3, ntt_const[15], ntt_const[NC+15], p);
    b4 = sp_simd_ntt_mul(b4, ntt_const[16], ntt_const[NC+16], p);
    b5 = sp_simd_ntt_mul(b5, ntt_const[17], ntt_const[NC+17], p);

    b1 = sp_simd_add(b0, b1, p);

    c1 = sp_simd_add(b1, b2, p);
    c2 = sp_simd_sub(b1, b2, p);
    c3 = sp_simd_add(b3, b5, p);
    c4 = sp_simd_add(b4, b5, p);

    b1 = sp_simd_add(c1, c3, p);
    b2 = sp_simd_add(c2, c4, p);
    b3 = sp_simd_sub(c1, c3, p);
    b4 = sp_simd_sub(c2, c4, p);

    a02 = b0;
    a05 = b4;
    a08 = b3;
    a11 = b1;
    a14 = b2;
  }
  {
    sp_simd_t x00, x01, x02;

    a01 = sp_simd_add(a00, a01, p);

    x00 = a00;
    x01 = sp_simd_add(a01, a02, p);
    x02 = sp_simd_sub(a01, a02, p);

    sp_simd_pfa_scatter(x00, x, j00, inc2, n);
    sp_simd_pfa_scatter(x02, x, j05, inc2, n);
    sp_simd_pfa_scatter(x01, x, j10, inc2, n);
  }
  {
    sp_simd_t x03, x04, x05;

    a04 = sp_simd_add(a03, a04, p);

    x03 = a03;
    x04 = sp_simd_add(a04, a05, p);
    x05 = sp_simd_sub(a04, a05, p);

    sp_simd_pfa_scatter(x04, x, j01, inc2, n);
    sp_simd_pfa_scatter(x03, x, j06, inc2, n);
    sp_simd_pfa_scatter(x05, x, j11, inc2, n);
  }
  {
    sp_simd_t x06, x07, x08;

    a07 = sp_simd_add(a06, a07, p);

    x06 = a06;
    x07 = sp_simd_add(a07, a08, p);
    x08 = sp_simd_sub(a07, a08, p);

    sp_simd_pfa_scatter(x08, x, j02, inc2, n);
    sp_simd_pfa_scatter(x07, x, j07, inc2, n);
    sp_simd_pfa_scatter(x06, x, j12, inc2, n);
  }
  {
    sp_simd_t x09, x10, x11;

    a10 = sp_simd_add(a09, a10, p);

    x09 = a09;
    x10 = sp_simd_add(a10, a11, p);
    x11 = sp_simd_sub(a10, a11, p);

    sp_simd_pfa_scatter(x09, x, j03, inc2, n);
    sp_simd_pfa_scatter(x11, x, j08, inc2, n);
    sp_simd_pfa_scatter(x10, x, j13, inc2, n);
  }
  {
    sp_simd_t x12, x13, x14;

    a13 = sp_simd_add(a12, a13, p);

    x12 = a12;
    x13 = sp_simd_add(a13, a14, p);
    x14 = sp_simd_sub(a13, a14, p);

    sp_simd_pfa_scatter(x13, x, j04, inc2, n);
    sp_simd_pfa_scatter(x12, x, j09, inc2, n);
    sp_simd_pfa_scatter(x14, x, j14, inc2, n);
  }
}
#endif

static void
ntt15_pfa_run(spv_t x, spv_size_t cofactor,
	  sp_t p, spv_t ntt_const)
{
  spv_size_t i = 0;
  spv_size_t incstart = 0;
  spv_size_t n = 15 * cofactor;
  spv_size_t inc = cofactor;
  spv_size_t inc2 = 15;

#ifdef HAVE_SSE2
  spv_size_t num_simd = SP_SIMD_VSIZE * (cofactor / SP_SIMD_VSIZE);

  for (i = 0; i < num_simd; i += SP_SIMD_VSIZE)
    {
      ntt15_pfa_run_core_simd(x, incstart, inc, inc2, n, p, ntt_const);
      incstart += SP_SIMD_VSIZE * inc2;
    }
#endif

  for (; i < cofactor; i++, incstart += inc2)
    ntt15_pfa_run_core(x, incstart, inc, n, p, ntt_const);

}

const nttconfig_t ntt15_config = 
{
  15,
  ntt15_get_num_const,
  ntt15_init,
  ntt15_run,
  ntt15_pfa_run,
  ntt15_twiddle_run
};
