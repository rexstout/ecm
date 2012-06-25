#include "ntt-impl.h"

#define NC 18

static uint32_t 
ntt16_get_num_const(void)
{
  return NC;
}

void
ntt16_init(spv_t out, sp_t p, sp_t d, 
	  sp_t primroot, sp_t order)
{
  uint32_t i;
  sp_t w[16];
  sp_t inv2 = sp_inv(2, p, d);
  sp_t inv4 = sp_sqr(inv2, p, d);
  sp_t t0, t1, t2, t3, t4, t5;
  sp_t p0, p1, p2, p3;

  w[1] = sp_pow(primroot, order / 16, p, d);
  for (i = 2; i < 16; i++)
    w[i] = sp_mul(w[i-1], w[1], p, d);

  t3 = sp_sub(w[1], w[9], p);
  t2 = sp_sub(w[15], w[7], p);
  t1 = sp_sub(w[5], w[13], p);
  t0 = sp_sub(w[11], w[3], p);

  p0 = sp_add(t0, t1, p);
  p1 = sp_sub(t0, t1, p);
  p2 = sp_add(t2, t3, p);
  p3 = sp_sub(t2, t3, p);

  t0 = sp_sub(p0, p2, p);
  t1 = sp_sub(p1, p3, p);
  t2 = sp_neg(sp_add(p0, p2, p), p);
  t3 = sp_neg(sp_add(p1, p3, p), p);
  t4 = p2;
  t5 = p3;

  out[0] = 1;
  out[1] = 1;
  out[2] = 1;
  out[3] = w[4];
  out[4] = 1;
  out[5] = w[4];
  out[6] = sp_mul(inv2, sp_sub(w[2], w[6], p), p, d);
  out[7] = sp_mul(inv2, sp_add(w[2], w[6], p), p, d);
  out[8] = 1;
  out[9] = w[4];
  out[10] = out[6];
  out[11] = out[7];
  out[12] = sp_mul(inv4, t0, p, d);
  out[13] = sp_mul(inv4, t1, p, d);
  out[14] = sp_mul(inv4, t2, p, d);
  out[15] = sp_mul(inv4, t3, p, d);
  out[16] = sp_mul(inv4, t4, p, d);
  out[17] = sp_mul(inv4, t5, p, d);
}

static void
ntt16_run(spv_t x, spv_size_t stride,
	  sp_t p, spv_t ntt_const)
{
  sp_t x0, x1, x2, x3, x4, x5, x6, x7,
       x8, x9, x10, x11, x12, x13, x14, x15;
  sp_t t0, t1, t2, t3, t4, t5, t6, t7, 
       t8, t9, t10, t11, t12, t13, t14, t15, t16, t17;
  sp_t p0, p1, p2, p3, p4, p5, p6, p7,
       p8, p9, p10, p11, p12, p13, p14, p15, p16, p17;

  #ifdef HAVE_PARTIAL_MOD
  p *= 2;
  #endif

  x0 = x[0 * stride];
  x1 = x[1 * stride];
  x2 = x[2 * stride];
  x3 = x[3 * stride];
  x4 = x[4 * stride];
  x5 = x[5 * stride];
  x6 = x[6 * stride];
  x7 = x[7 * stride];
  x8 = x[8 * stride];
  x9 = x[9 * stride];
  x10 = x[10 * stride];
  x11 = x[11 * stride];
  x12 = x[12 * stride];
  x13 = x[13 * stride];
  x14 = x[14 * stride];
  x15 = x[15 * stride];

  t0 = sp_add(x0, x8, p);
  t8 = sp_sub(x0, x8, p);
  t1 = sp_add(x1, x9, p);
  t9 = sp_sub(x1, x9, p);
  t2 = sp_add(x2, x10, p);
  t10 = sp_sub(x2, x10, p);
  t3 = sp_add(x3, x11, p);
  t11 = sp_sub(x3, x11, p);
  t4 = sp_add(x4, x12, p);
  t12 = sp_sub_partial(x4, x12, p);
  t5 = sp_add(x5, x13, p);
  t13 = sp_sub(x13, x5, p);
  t6 = sp_add(x6, x14, p);
  t14 = sp_sub(x14, x6, p);
  t7 = sp_add(x7, x15, p);
  t15 = sp_sub(x15, x7, p);

  p0 = sp_add(t0, t4, p);
  p4 = sp_sub(t0, t4, p);
  p1 = sp_add(t1, t5, p);
  p5 = sp_sub(t1, t5, p);
  p2 = sp_add(t2, t6, p);
  p6 = sp_sub_partial(t2, t6, p);
  p3 = sp_add(t3, t7, p);
  p7 = sp_sub(t3, t7, p);
  p8 = t8;
  p9 = t12;
  p10 = sp_add_partial(t10, t14, p);
  p11 = sp_sub_partial(t10, t14, p);
  p12 = sp_add(t9, t15, p);
  p13 = sp_sub(t9, t15, p);
  p14 = sp_add(t13, t11, p);
  p15 = sp_sub(t13, t11, p);

  t0 = sp_add(p0, p2, p);
  t1 = sp_sub(p0, p2, p);
  t2 = sp_add(p1, p3, p);
  t3 = sp_sub_partial(p1, p3, p);
  t4 = p4;
  t5 = p6;
  t6 = sp_sub_partial(p5, p7, p);
  t7 = sp_add_partial(p5, p7, p); 
  t8 = p8;
  t9 = p9;
  t10 = p10;
  t11 = p11;
  t12 = p12;
  t13 = p13;
  t14 = p14;
  t15 = p15;
  t16 = sp_add_partial(p12, p14, p);
  t17 = sp_add_partial(p13, p15, p);

  t3 = sp_ntt_mul(t3, ntt_const[3], ntt_const[NC+3], p);
  t5 = sp_ntt_mul(t5, ntt_const[5], ntt_const[NC+5], p);
  t6 = sp_ntt_mul(t6, ntt_const[6], ntt_const[NC+6], p);
  t7 = sp_ntt_mul(t7, ntt_const[7], ntt_const[NC+7], p);
  t9 = sp_ntt_mul(t9, ntt_const[9], ntt_const[NC+9], p);
  t10 = sp_ntt_mul(t10, ntt_const[10], ntt_const[NC+10], p);
  t11 = sp_ntt_mul(t11, ntt_const[11], ntt_const[NC+11], p);
  t12 = sp_ntt_mul(t12, ntt_const[12], ntt_const[NC+12], p);
  t13 = sp_ntt_mul(t13, ntt_const[13], ntt_const[NC+13], p);
  t14 = sp_ntt_mul(t14, ntt_const[14], ntt_const[NC+14], p);
  t15 = sp_ntt_mul(t15, ntt_const[15], ntt_const[NC+15], p);
  t16 = sp_ntt_mul(t16, ntt_const[16], ntt_const[NC+16], p);
  t17 = sp_ntt_mul(t17, ntt_const[17], ntt_const[NC+17], p);

  p0 = sp_add(t4, t5, p);
  p1 = sp_sub(t4, t5, p);
  p2 = sp_add(t6, t7, p);
  p3 = sp_sub(t6, t7, p);
  p4 = sp_add(p0, p2, p);
  p5 = sp_sub(p0, p2, p);
  p6 = sp_add(p1, p3, p);
  p7 = sp_sub(p1, p3, p);
  p0 = sp_add(t0, t2, p);
  p1 = sp_sub(t0, t2, p);
  p2 = sp_add(t1, t3, p);
  p3 = sp_sub(t1, t3, p);

  t0 = sp_add(t12, t16, p);
  t1 = sp_add(t13, t17, p);
  t2 = sp_add(t14, t16, p);
  t3 = sp_add(t15, t17, p);
  t7 = sp_add(t0, t1, p);
  t6 = sp_sub(t0, t1, p);
  t5 = sp_add(t2, t3, p);
  t4 = sp_sub(t2, t3, p); 

  t2 = sp_add(t10, t11, p);
  t3 = sp_sub(t10, t11, p);



  t10 = sp_add(t8, t9, p);
  t11 = sp_sub(t8, t9, p);

  t12 = sp_add(t10, t2, p);
  t13 = sp_sub(t10, t2, p);
  t14 = sp_add(t11, t3, p);
  t15 = sp_sub(t11, t3, p);

  p8 = sp_add(t12, t4, p);
  p9 = sp_sub(t12, t4, p);
  p10 = sp_add(t14, t5, p);
  p11 = sp_sub(t14, t5, p);
  p12 = sp_add(t13, t6, p);
  p13 = sp_sub(t13, t6, p);
  p14 = sp_add(t15, t7, p);
  p15 = sp_sub(t15, t7, p);

  x[ 0 * stride] = p0;
  x[ 1 * stride] = p8;
  x[ 2 * stride] = p4;
  x[ 3 * stride] = p15;
  x[ 4 * stride] = p2;
  x[ 5 * stride] = p12;
  x[ 6 * stride] = p7;
  x[ 7 * stride] = p11;
  x[ 8 * stride] = p1;
  x[ 9 * stride] = p9;
  x[10 * stride] = p5;
  x[11 * stride] = p14;
  x[12 * stride] = p3;
  x[13 * stride] = p13;
  x[14 * stride] = p6;
  x[15 * stride] = p10;
}


#ifdef HAVE_SSE2
static void
ntt16_run_simd(spv_t x, spv_size_t stride,
	  sp_t p, spv_t ntt_const)
{
  sp_simd_t x0, x1, x2, x3, x4, x5, x6, x7,
            x8, x9, x10, x11, x12, x13, x14, x15;
  sp_simd_t t0, t1, t2, t3, t4, t5, t6, t7, 
            t8, t9, t10, t11, t12, t13, t14, t15, t16, t17;
  sp_simd_t p0, p1, p2, p3, p4, p5, p6, p7,
            p8, p9, p10, p11, p12, p13, p14, p15, p16, p17;

  #ifdef HAVE_PARTIAL_MOD
  p *= 2;
  #endif

  x0 = sp_simd_gather(x + 0 * stride);
  x1 = sp_simd_gather(x + 1 * stride);
  x2 = sp_simd_gather(x + 2 * stride);
  x3 = sp_simd_gather(x + 3 * stride);
  x4 = sp_simd_gather(x + 4 * stride);
  x5 = sp_simd_gather(x + 5 * stride);
  x6 = sp_simd_gather(x + 6 * stride);
  x7 = sp_simd_gather(x + 7 * stride);
  x8 = sp_simd_gather(x + 8 * stride);
  x9 = sp_simd_gather(x + 9 * stride);
  x10 = sp_simd_gather(x + 10 * stride);
  x11 = sp_simd_gather(x + 11 * stride);
  x12 = sp_simd_gather(x + 12 * stride);
  x13 = sp_simd_gather(x + 13 * stride);
  x14 = sp_simd_gather(x + 14 * stride);
  x15 = sp_simd_gather(x + 15 * stride);

  t0 = sp_simd_add(x0, x8, p);
  t8 = sp_simd_sub(x0, x8, p);
  t1 = sp_simd_add(x1, x9, p);
  t9 = sp_simd_sub(x1, x9, p);
  t2 = sp_simd_add(x2, x10, p);
  t10 = sp_simd_sub(x2, x10, p);
  t3 = sp_simd_add(x3, x11, p);
  t11 = sp_simd_sub(x3, x11, p);
  t4 = sp_simd_add(x4, x12, p);
  t12 = sp_simd_sub_partial(x4, x12, p);
  t5 = sp_simd_add(x5, x13, p);
  t13 = sp_simd_sub(x13, x5, p);
  t6 = sp_simd_add(x6, x14, p);
  t14 = sp_simd_sub(x14, x6, p);
  t7 = sp_simd_add(x7, x15, p);
  t15 = sp_simd_sub(x15, x7, p);

  p0 = sp_simd_add(t0, t4, p);
  p4 = sp_simd_sub(t0, t4, p);
  p1 = sp_simd_add(t1, t5, p);
  p5 = sp_simd_sub(t1, t5, p);
  p2 = sp_simd_add(t2, t6, p);
  p6 = sp_simd_sub_partial(t2, t6, p);
  p3 = sp_simd_add(t3, t7, p);
  p7 = sp_simd_sub(t3, t7, p);
  p8 = t8;
  p9 = t12;
  p10 = sp_simd_add_partial(t10, t14, p);
  p11 = sp_simd_sub_partial(t10, t14, p);
  p12 = sp_simd_add(t9, t15, p);
  p13 = sp_simd_sub(t9, t15, p);
  p14 = sp_simd_add(t13, t11, p);
  p15 = sp_simd_sub(t13, t11, p);

  t0 = sp_simd_add(p0, p2, p);
  t1 = sp_simd_sub(p0, p2, p);
  t2 = sp_simd_add(p1, p3, p);
  t3 = sp_simd_sub_partial(p1, p3, p);
  t4 = p4;
  t5 = p6;
  t6 = sp_simd_sub_partial(p5, p7, p);
  t7 = sp_simd_add_partial(p5, p7, p); 
  t8 = p8;
  t9 = p9;
  t10 = p10;
  t11 = p11;
  t12 = p12;
  t13 = p13;
  t14 = p14;
  t15 = p15;
  t16 = sp_simd_add_partial(p12, p14, p);
  t17 = sp_simd_add_partial(p13, p15, p);

  t3 = sp_simd_ntt_mul(t3, ntt_const[3], ntt_const[NC+3], p);
  t5 = sp_simd_ntt_mul(t5, ntt_const[5], ntt_const[NC+5], p);
  t6 = sp_simd_ntt_mul(t6, ntt_const[6], ntt_const[NC+6], p);
  t7 = sp_simd_ntt_mul(t7, ntt_const[7], ntt_const[NC+7], p);
  t9 = sp_simd_ntt_mul(t9, ntt_const[9], ntt_const[NC+9], p);
  t10 = sp_simd_ntt_mul(t10, ntt_const[10], ntt_const[NC+10], p);
  t11 = sp_simd_ntt_mul(t11, ntt_const[11], ntt_const[NC+11], p);
  t12 = sp_simd_ntt_mul(t12, ntt_const[12], ntt_const[NC+12], p);
  t13 = sp_simd_ntt_mul(t13, ntt_const[13], ntt_const[NC+13], p);
  t14 = sp_simd_ntt_mul(t14, ntt_const[14], ntt_const[NC+14], p);
  t15 = sp_simd_ntt_mul(t15, ntt_const[15], ntt_const[NC+15], p);
  t16 = sp_simd_ntt_mul(t16, ntt_const[16], ntt_const[NC+16], p);
  t17 = sp_simd_ntt_mul(t17, ntt_const[17], ntt_const[NC+17], p);

  p0 = sp_simd_add(t4, t5, p);
  p1 = sp_simd_sub(t4, t5, p);
  p2 = sp_simd_add(t6, t7, p);
  p3 = sp_simd_sub(t6, t7, p);
  p4 = sp_simd_add(p0, p2, p);
  p5 = sp_simd_sub(p0, p2, p);
  p6 = sp_simd_add(p1, p3, p);
  p7 = sp_simd_sub(p1, p3, p);
  p0 = sp_simd_add(t0, t2, p);
  p1 = sp_simd_sub(t0, t2, p);
  p2 = sp_simd_add(t1, t3, p);
  p3 = sp_simd_sub(t1, t3, p);

  t0 = sp_simd_add(t12, t16, p);
  t1 = sp_simd_add(t13, t17, p);
  t2 = sp_simd_add(t14, t16, p);
  t3 = sp_simd_add(t15, t17, p);
  t7 = sp_simd_add(t0, t1, p);
  t6 = sp_simd_sub(t0, t1, p);
  t5 = sp_simd_add(t2, t3, p);
  t4 = sp_simd_sub(t2, t3, p); 

  t2 = sp_simd_add(t10, t11, p);
  t3 = sp_simd_sub(t10, t11, p);



  t10 = sp_simd_add(t8, t9, p);
  t11 = sp_simd_sub(t8, t9, p);

  t12 = sp_simd_add(t10, t2, p);
  t13 = sp_simd_sub(t10, t2, p);
  t14 = sp_simd_add(t11, t3, p);
  t15 = sp_simd_sub(t11, t3, p);

  p8 = sp_simd_add(t12, t4, p);
  p9 = sp_simd_sub(t12, t4, p);
  p10 = sp_simd_add(t14, t5, p);
  p11 = sp_simd_sub(t14, t5, p);
  p12 = sp_simd_add(t13, t6, p);
  p13 = sp_simd_sub(t13, t6, p);
  p14 = sp_simd_add(t15, t7, p);
  p15 = sp_simd_sub(t15, t7, p);

  sp_simd_scatter(p0,  x +  0 * stride);
  sp_simd_scatter(p8,  x +  1 * stride);
  sp_simd_scatter(p4,  x +  2 * stride);
  sp_simd_scatter(p15, x +  3 * stride);
  sp_simd_scatter(p2,  x +  4 * stride);
  sp_simd_scatter(p12, x +  5 * stride);
  sp_simd_scatter(p7,  x +  6 * stride);
  sp_simd_scatter(p11, x +  7 * stride);
  sp_simd_scatter(p1,  x +  8 * stride);
  sp_simd_scatter(p9,  x +  9 * stride);
  sp_simd_scatter(p5,  x + 10 * stride);
  sp_simd_scatter(p14, x + 11 * stride);
  sp_simd_scatter(p3,  x + 12 * stride);
  sp_simd_scatter(p13, x + 13 * stride);
  sp_simd_scatter(p6,  x + 14 * stride);
  sp_simd_scatter(p10, x + 15 * stride);
}
#endif

static void
ntt16_twiddle_run(spv_t x, spv_size_t stride,
	  spv_size_t num_transforms,
	  sp_t p, spv_t ntt_const)
{
  spv_size_t i = 0;

#ifdef HAVE_SSE2
  spv_size_t num_simd = SP_SIMD_VSIZE * (num_transforms / SP_SIMD_VSIZE);

  for (i = 0; i < num_simd; i += SP_SIMD_VSIZE)
      ntt16_run_simd(x + i, stride, p, ntt_const);
#endif

  for (; i < num_transforms; i++)
    ntt16_run(x + i, stride, p, ntt_const);
}


static void
ntt16_pfa_run_core(spv_t x, spv_size_t start,
	  spv_size_t inc, spv_size_t n,
	  sp_t p, spv_t ntt_const)
{
  spv_size_t j0, j1, j2, j3, j4, j5, j6, j7,
             j8, j9, j10, j11, j12, j13, j14, j15;
  sp_t x0, x1, x2, x3, x4, x5, x6, x7,
       x8, x9, x10, x11, x12, x13, x14, x15;
  sp_t t0, t1, t2, t3, t4, t5, t6, t7, 
       t8, t9, t10, t11, t12, t13, t14, t15, t16, t17;
  sp_t p0, p1, p2, p3, p4, p5, p6, p7,
       p8, p9, p10, p11, p12, p13, p14, p15, p16, p17;

  #ifdef HAVE_PARTIAL_MOD
  p *= 2;
  #endif

  j0 = start;
  j1 = sp_array_inc(j0, inc, n);
  j2 = sp_array_inc(j0, 2 * inc, n);
  j3 = sp_array_inc(j0, 3 * inc, n);
  j4 = sp_array_inc(j0, 4 * inc, n);
  j5 = sp_array_inc(j0, 5 * inc, n);
  j6 = sp_array_inc(j0, 6 * inc, n);
  j7 = sp_array_inc(j0, 7 * inc, n);
  j8 = sp_array_inc(j0, 8 * inc, n);
  j9 = sp_array_inc(j0, 9 * inc, n);
  j10 = sp_array_inc(j0, 10 * inc, n);
  j11 = sp_array_inc(j0, 11 * inc, n);
  j12 = sp_array_inc(j0, 12 * inc, n);
  j13 = sp_array_inc(j0, 13 * inc, n);
  j14 = sp_array_inc(j0, 14 * inc, n);
  j15 = sp_array_inc(j0, 15 * inc, n);

  x0 = x[j0];
  x1 = x[j1];
  x2 = x[j2];
  x3 = x[j3];
  x4 = x[j4];
  x5 = x[j5];
  x6 = x[j6];
  x7 = x[j7];
  x8 = x[j8];
  x9 = x[j9];
  x10 = x[j10];
  x11 = x[j11];
  x12 = x[j12];
  x13 = x[j13];
  x14 = x[j14];
  x15 = x[j15];

  t0 = sp_add(x0, x8, p);
  t8 = sp_sub(x0, x8, p);
  t1 = sp_add(x1, x9, p);
  t9 = sp_sub(x1, x9, p);
  t2 = sp_add(x2, x10, p);
  t10 = sp_sub(x2, x10, p);
  t3 = sp_add(x3, x11, p);
  t11 = sp_sub(x3, x11, p);
  t4 = sp_add(x4, x12, p);
  t12 = sp_sub_partial(x4, x12, p);
  t5 = sp_add(x5, x13, p);
  t13 = sp_sub(x13, x5, p);
  t6 = sp_add(x6, x14, p);
  t14 = sp_sub(x14, x6, p);
  t7 = sp_add(x7, x15, p);
  t15 = sp_sub(x15, x7, p);

  p0 = sp_add(t0, t4, p);
  p4 = sp_sub(t0, t4, p);
  p1 = sp_add(t1, t5, p);
  p5 = sp_sub(t1, t5, p);
  p2 = sp_add(t2, t6, p);
  p6 = sp_sub_partial(t2, t6, p);
  p3 = sp_add(t3, t7, p);
  p7 = sp_sub(t3, t7, p);
  p8 = t8;
  p9 = t12;
  p10 = sp_add_partial(t10, t14, p);
  p11 = sp_sub_partial(t10, t14, p);
  p12 = sp_add(t9, t15, p);
  p13 = sp_sub(t9, t15, p);
  p14 = sp_add(t13, t11, p);
  p15 = sp_sub(t13, t11, p);

  t0 = sp_add(p0, p2, p);
  t1 = sp_sub(p0, p2, p);
  t2 = sp_add(p1, p3, p);
  t3 = sp_sub_partial(p1, p3, p);
  t4 = p4;
  t5 = p6;
  t6 = sp_sub_partial(p5, p7, p);
  t7 = sp_add_partial(p5, p7, p); 
  t8 = p8;
  t9 = p9;
  t10 = p10;
  t11 = p11;
  t12 = p12;
  t13 = p13;
  t14 = p14;
  t15 = p15;
  t16 = sp_add_partial(p12, p14, p);
  t17 = sp_add_partial(p13, p15, p);

  t3 = sp_ntt_mul(t3, ntt_const[3], ntt_const[NC+3], p);
  t5 = sp_ntt_mul(t5, ntt_const[5], ntt_const[NC+5], p);
  t6 = sp_ntt_mul(t6, ntt_const[6], ntt_const[NC+6], p);
  t7 = sp_ntt_mul(t7, ntt_const[7], ntt_const[NC+7], p);
  t9 = sp_ntt_mul(t9, ntt_const[9], ntt_const[NC+9], p);
  t10 = sp_ntt_mul(t10, ntt_const[10], ntt_const[NC+10], p);
  t11 = sp_ntt_mul(t11, ntt_const[11], ntt_const[NC+11], p);
  t12 = sp_ntt_mul(t12, ntt_const[12], ntt_const[NC+12], p);
  t13 = sp_ntt_mul(t13, ntt_const[13], ntt_const[NC+13], p);
  t14 = sp_ntt_mul(t14, ntt_const[14], ntt_const[NC+14], p);
  t15 = sp_ntt_mul(t15, ntt_const[15], ntt_const[NC+15], p);
  t16 = sp_ntt_mul(t16, ntt_const[16], ntt_const[NC+16], p);
  t17 = sp_ntt_mul(t17, ntt_const[17], ntt_const[NC+17], p);

  p0 = sp_add(t4, t5, p);
  p1 = sp_sub(t4, t5, p);
  p2 = sp_add(t6, t7, p);
  p3 = sp_sub(t6, t7, p);
  p4 = sp_add(p0, p2, p);
  p5 = sp_sub(p0, p2, p);
  p6 = sp_add(p1, p3, p);
  p7 = sp_sub(p1, p3, p);
  p0 = sp_add(t0, t2, p);
  p1 = sp_sub(t0, t2, p);
  p2 = sp_add(t1, t3, p);
  p3 = sp_sub(t1, t3, p);

  t0 = sp_add(t12, t16, p);
  t1 = sp_add(t13, t17, p);
  t2 = sp_add(t14, t16, p);
  t3 = sp_add(t15, t17, p);
  t7 = sp_add(t0, t1, p);
  t6 = sp_sub(t0, t1, p);
  t5 = sp_add(t2, t3, p);
  t4 = sp_sub(t2, t3, p); 

  t2 = sp_add(t10, t11, p);
  t3 = sp_sub(t10, t11, p);



  t10 = sp_add(t8, t9, p);
  t11 = sp_sub(t8, t9, p);

  t12 = sp_add(t10, t2, p);
  t13 = sp_sub(t10, t2, p);
  t14 = sp_add(t11, t3, p);
  t15 = sp_sub(t11, t3, p);

  p8 = sp_add(t12, t4, p);
  p9 = sp_sub(t12, t4, p);
  p10 = sp_add(t14, t5, p);
  p11 = sp_sub(t14, t5, p);
  p12 = sp_add(t13, t6, p);
  p13 = sp_sub(t13, t6, p);
  p14 = sp_add(t15, t7, p);
  p15 = sp_sub(t15, t7, p);

  x[j0] = p0;
  x[j1] = p8;
  x[j2] = p4;
  x[j3] = p15;
  x[j4] = p2;
  x[j5] = p12;
  x[j6] = p7;
  x[j7] = p11;
  x[j8] = p1;
  x[j9] = p9;
  x[j10] = p5;
  x[j11] = p14;
  x[j12] = p3;
  x[j13] = p13;
  x[j14] = p6;
  x[j15] = p10;
}

#ifdef HAVE_SSE2
static void
ntt16_pfa_run_core_simd(spv_t x, spv_size_t start,
	  spv_size_t inc, spv_size_t inc2, spv_size_t n,
	  sp_t p, spv_t ntt_const)
{
  spv_size_t j0, j1, j2, j3, j4, j5, j6, j7,
             j8, j9, j10, j11, j12, j13, j14, j15;
  sp_simd_t x0, x1, x2, x3, x4, x5, x6, x7,
            x8, x9, x10, x11, x12, x13, x14, x15;
  sp_simd_t t0, t1, t2, t3, t4, t5, t6, t7, 
            t8, t9, t10, t11, t12, t13, t14, t15, t16, t17;
  sp_simd_t p0, p1, p2, p3, p4, p5, p6, p7,
            p8, p9, p10, p11, p12, p13, p14, p15, p16, p17;

  #ifdef HAVE_PARTIAL_MOD
  p *= 2;
  #endif

  j0 = start;
  j1 = sp_array_inc(j0, inc, n);
  j2 = sp_array_inc(j0, 2 * inc, n);
  j3 = sp_array_inc(j0, 3 * inc, n);
  j4 = sp_array_inc(j0, 4 * inc, n);
  j5 = sp_array_inc(j0, 5 * inc, n);
  j6 = sp_array_inc(j0, 6 * inc, n);
  j7 = sp_array_inc(j0, 7 * inc, n);
  j8 = sp_array_inc(j0, 8 * inc, n);
  j9 = sp_array_inc(j0, 9 * inc, n);
  j10 = sp_array_inc(j0, 10 * inc, n);
  j11 = sp_array_inc(j0, 11 * inc, n);
  j12 = sp_array_inc(j0, 12 * inc, n);
  j13 = sp_array_inc(j0, 13 * inc, n);
  j14 = sp_array_inc(j0, 14 * inc, n);
  j15 = sp_array_inc(j0, 15 * inc, n);

  x0 = sp_simd_pfa_gather(x, j0, inc2, n);
  x1 = sp_simd_pfa_gather(x, j1, inc2, n);
  x2 = sp_simd_pfa_gather(x, j2, inc2, n);
  x3 = sp_simd_pfa_gather(x, j3, inc2, n);
  x4 = sp_simd_pfa_gather(x, j4, inc2, n);
  x5 = sp_simd_pfa_gather(x, j5, inc2, n);
  x6 = sp_simd_pfa_gather(x, j6, inc2, n);
  x7 = sp_simd_pfa_gather(x, j7, inc2, n);
  x8 = sp_simd_pfa_gather(x, j8, inc2, n);
  x9 = sp_simd_pfa_gather(x, j9, inc2, n);
  x10 = sp_simd_pfa_gather(x, j10, inc2, n);
  x11 = sp_simd_pfa_gather(x, j11, inc2, n);
  x12 = sp_simd_pfa_gather(x, j12, inc2, n);
  x13 = sp_simd_pfa_gather(x, j13, inc2, n);
  x14 = sp_simd_pfa_gather(x, j14, inc2, n);
  x15 = sp_simd_pfa_gather(x, j15, inc2, n);

  t0 = sp_simd_add(x0, x8, p);
  t8 = sp_simd_sub(x0, x8, p);
  t1 = sp_simd_add(x1, x9, p);
  t9 = sp_simd_sub(x1, x9, p);
  t2 = sp_simd_add(x2, x10, p);
  t10 = sp_simd_sub(x2, x10, p);
  t3 = sp_simd_add(x3, x11, p);
  t11 = sp_simd_sub(x3, x11, p);
  t4 = sp_simd_add(x4, x12, p);
  t12 = sp_simd_sub_partial(x4, x12, p);
  t5 = sp_simd_add(x5, x13, p);
  t13 = sp_simd_sub(x13, x5, p);
  t6 = sp_simd_add(x6, x14, p);
  t14 = sp_simd_sub(x14, x6, p);
  t7 = sp_simd_add(x7, x15, p);
  t15 = sp_simd_sub(x15, x7, p);

  p0 = sp_simd_add(t0, t4, p);
  p4 = sp_simd_sub(t0, t4, p);
  p1 = sp_simd_add(t1, t5, p);
  p5 = sp_simd_sub(t1, t5, p);
  p2 = sp_simd_add(t2, t6, p);
  p6 = sp_simd_sub_partial(t2, t6, p);
  p3 = sp_simd_add(t3, t7, p);
  p7 = sp_simd_sub(t3, t7, p);
  p8 = t8;
  p9 = t12;
  p10 = sp_simd_add_partial(t10, t14, p);
  p11 = sp_simd_sub_partial(t10, t14, p);
  p12 = sp_simd_add(t9, t15, p);
  p13 = sp_simd_sub(t9, t15, p);
  p14 = sp_simd_add(t13, t11, p);
  p15 = sp_simd_sub(t13, t11, p);

  t0 = sp_simd_add(p0, p2, p);
  t1 = sp_simd_sub(p0, p2, p);
  t2 = sp_simd_add(p1, p3, p);
  t3 = sp_simd_sub_partial(p1, p3, p);
  t4 = p4;
  t5 = p6;
  t6 = sp_simd_sub_partial(p5, p7, p);
  t7 = sp_simd_add_partial(p5, p7, p); 
  t8 = p8;
  t9 = p9;
  t10 = p10;
  t11 = p11;
  t12 = p12;
  t13 = p13;
  t14 = p14;
  t15 = p15;
  t16 = sp_simd_add_partial(p12, p14, p);
  t17 = sp_simd_add_partial(p13, p15, p);

  t3 = sp_simd_ntt_mul(t3, ntt_const[3], ntt_const[NC+3], p);
  t5 = sp_simd_ntt_mul(t5, ntt_const[5], ntt_const[NC+5], p);
  t6 = sp_simd_ntt_mul(t6, ntt_const[6], ntt_const[NC+6], p);
  t7 = sp_simd_ntt_mul(t7, ntt_const[7], ntt_const[NC+7], p);
  t9 = sp_simd_ntt_mul(t9, ntt_const[9], ntt_const[NC+9], p);
  t10 = sp_simd_ntt_mul(t10, ntt_const[10], ntt_const[NC+10], p);
  t11 = sp_simd_ntt_mul(t11, ntt_const[11], ntt_const[NC+11], p);
  t12 = sp_simd_ntt_mul(t12, ntt_const[12], ntt_const[NC+12], p);
  t13 = sp_simd_ntt_mul(t13, ntt_const[13], ntt_const[NC+13], p);
  t14 = sp_simd_ntt_mul(t14, ntt_const[14], ntt_const[NC+14], p);
  t15 = sp_simd_ntt_mul(t15, ntt_const[15], ntt_const[NC+15], p);
  t16 = sp_simd_ntt_mul(t16, ntt_const[16], ntt_const[NC+16], p);
  t17 = sp_simd_ntt_mul(t17, ntt_const[17], ntt_const[NC+17], p);

  p0 = sp_simd_add(t4, t5, p);
  p1 = sp_simd_sub(t4, t5, p);
  p2 = sp_simd_add(t6, t7, p);
  p3 = sp_simd_sub(t6, t7, p);
  p4 = sp_simd_add(p0, p2, p);
  p5 = sp_simd_sub(p0, p2, p);
  p6 = sp_simd_add(p1, p3, p);
  p7 = sp_simd_sub(p1, p3, p);
  p0 = sp_simd_add(t0, t2, p);
  p1 = sp_simd_sub(t0, t2, p);
  p2 = sp_simd_add(t1, t3, p);
  p3 = sp_simd_sub(t1, t3, p);

  t0 = sp_simd_add(t12, t16, p);
  t1 = sp_simd_add(t13, t17, p);
  t2 = sp_simd_add(t14, t16, p);
  t3 = sp_simd_add(t15, t17, p);
  t7 = sp_simd_add(t0, t1, p);
  t6 = sp_simd_sub(t0, t1, p);
  t5 = sp_simd_add(t2, t3, p);
  t4 = sp_simd_sub(t2, t3, p); 

  t2 = sp_simd_add(t10, t11, p);
  t3 = sp_simd_sub(t10, t11, p);



  t10 = sp_simd_add(t8, t9, p);
  t11 = sp_simd_sub(t8, t9, p);

  t12 = sp_simd_add(t10, t2, p);
  t13 = sp_simd_sub(t10, t2, p);
  t14 = sp_simd_add(t11, t3, p);
  t15 = sp_simd_sub(t11, t3, p);

  p8 = sp_simd_add(t12, t4, p);
  p9 = sp_simd_sub(t12, t4, p);
  p10 = sp_simd_add(t14, t5, p);
  p11 = sp_simd_sub(t14, t5, p);
  p12 = sp_simd_add(t13, t6, p);
  p13 = sp_simd_sub(t13, t6, p);
  p14 = sp_simd_add(t15, t7, p);
  p15 = sp_simd_sub(t15, t7, p);

  sp_simd_pfa_scatter(p0, x, j0, inc2, n);
  sp_simd_pfa_scatter(p8, x, j1, inc2, n);
  sp_simd_pfa_scatter(p4, x, j2, inc2, n);
  sp_simd_pfa_scatter(p15,x, j3, inc2, n);
  sp_simd_pfa_scatter(p2, x, j4, inc2, n);
  sp_simd_pfa_scatter(p12,x, j5, inc2, n);
  sp_simd_pfa_scatter(p7, x, j6, inc2, n);
  sp_simd_pfa_scatter(p11,x, j7, inc2, n);
  sp_simd_pfa_scatter(p1, x, j8, inc2, n);
  sp_simd_pfa_scatter(p9, x, j9, inc2, n);
  sp_simd_pfa_scatter(p5, x, j10, inc2, n);
  sp_simd_pfa_scatter(p14,x, j11, inc2, n);
  sp_simd_pfa_scatter(p3, x, j12, inc2, n);
  sp_simd_pfa_scatter(p13,x, j13, inc2, n);
  sp_simd_pfa_scatter(p6, x, j14, inc2, n);
  sp_simd_pfa_scatter(p10,x, j15, inc2, n);
}
#endif

static void
ntt16_pfa_run(spv_t x, spv_size_t cofactor,
	  sp_t p, spv_t ntt_const)
{
  spv_size_t i = 0;
  spv_size_t incstart = 0;
  spv_size_t n = 16 * cofactor;
  spv_size_t inc = cofactor;
  spv_size_t inc2 = 16;

#ifdef HAVE_SSE2
  spv_size_t num_simd = SP_SIMD_VSIZE * (cofactor / SP_SIMD_VSIZE);

  for (i = 0; i < num_simd; i += SP_SIMD_VSIZE)
    {
      ntt16_pfa_run_core_simd(x, incstart, inc, inc2, n, p, ntt_const);
      incstart += SP_SIMD_VSIZE * inc2;
    }
#endif

  for (; i < cofactor; i++, incstart += inc2)
    ntt16_pfa_run_core(x, incstart, inc, n, p, ntt_const);

}

const nttconfig_t ntt16_config = 
{
  16,
  ntt16_get_num_const,
  ntt16_init,
  ntt16_run,
  ntt16_pfa_run,
  ntt16_twiddle_run
};
