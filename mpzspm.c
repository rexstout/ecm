/* mpzspm.c - "mpz small prime moduli" - pick a set of small primes large
   enough to represent a mpzp

  Copyright 2005 Dave Newman.

  The SP Library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or (at your
  option) any later version.

  The SP Library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
  License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with the SP Library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
  MA 02111-1307, USA.
*/

#include <malloc.h>
#include <stdio.h> /* for printf */
#include <time.h> /* FIXME  */
#include "sp.h"

mpzspm_t
mpzspm_init (mpz_t modulus, spv_size_t max_len)
{
  unsigned int ub, i, j;
  mpz_t P, S, T;
  sp_t p, a, d;
  mpzspm_t mpzspm;
  
  mpzspm = (mpzspm_t) malloc (sizeof (__mpzspm_struct));
  
  /* upper bound for the number of primes we need
   * = ceil((log_2(modulus^2) + log_2(max_len)) / log_2(p)),
   * where p is the minimum admissible prime */
  
  /* FIXME: bound broken */
  ub = /*3*/ 5 + (1 + 2 * mpz_sizeinbase (modulus, 2) + ceil_log_2 (max_len))
         / (GMP_NUMB_BITS - 1);

  mpzspm->spm = (spm_t) malloc (ub * sizeof (__spm_struct));
  mpzspm->sp_num = 0;

  /* product of primes selected so far */
  mpz_init_set_ui (P, 1);
  /* sum of primes selected so far */
  mpz_init (S);
  mpz_init (T); 
  mpz_mul (T, modulus, modulus);
  mpz_mul_ui (T, T, max_len);
  
  /* find primes congruent to 1 mod max_len so we can do
   * a ntt of size max_len
   * 
   * '+ max_len' cancels the first p -= max_len */
  p = ULONG_MAX - (ULONG_MAX % max_len) + max_len + 1;
  do
    {
      do
        p -= max_len;
      while (!sp_prime(p));
      
      /* all primes must have top bit set */
      if (p < (ULONG_MAX >> 1))
        {
	  printf ("not enough primes in interval\n");
	  return NULL;
	}
      
      mpzspm->spm[mpzspm->sp_num++].sp = p;
      
      mpz_mul_ui (P, P, p);
      mpz_add_ui (S, S, p);

      /* we want P > 4 * max_len * (modulus * S)^2 */
      mpz_mul (T, S, modulus);
      mpz_mul (T, T, T);
      mpz_mul_ui (T, T, max_len);
      mpz_add (T, T, T);
      mpz_add (T, T, T);
    }
  while (mpz_cmp (P, T) <= 0);

//  printf ("ub=%d, %d primes chosen\n", ub, mpzspm->sp_num);
  
  mpz_init_set (mpzspm->modulus, modulus);
  
  mpzspm->crt1 = (mpzp_t) malloc (mpzspm->sp_num * sizeof (mpz_t));
  mpzspm->crt2 = (mpzp_t) malloc ((mpzspm->sp_num + 2) * sizeof (mpz_t));
  mpzspm->crt3 = (spv_t) malloc (mpzspm->sp_num * sizeof (sp_t));
  mpzspm->crt4 = (spv_t *) malloc (mpzspm->sp_num * sizeof (sp_t *));
  mpzspm->crt5 = (spv_t) malloc (mpzspm->sp_num * sizeof (sp_t));

  for (i = 0; i < mpzspm->sp_num; i++)
    mpzspm->crt4[i] = (spv_t) malloc (mpzspm->sp_num * sizeof (sp_t));
  
  for (i = 0; i < mpzspm->sp_num; i++)
    {
      p = mpzspm->spm[i].sp;
      
      invert_limb (mpzspm->spm[i].mul_c, p);
      d = mpzspm->spm[i].mul_c;
      
      /* find a quadratic nonresidue mod p */
      for (a = 2; sp_pow (a, (p - 1) / 2, p, d) == 1; a++);

      /* we used to save only a primitive max_len'th root of unity
       * but just save the primitive root as it for now */
      
      mpzspm->spm[i].prim_root = a;

#if 0	  
      /* turn this into a primitive max_len'th root of unity mod p */
      mpzspm->spm[i].prim_root = sp_pow (a, (p - 1) / max_len, p, d);
#endif      

      /* crt3[i] = (P / p)^{-1} mod p */
      mpz_fdiv_q_ui (T, P, p);
      a = mpz_fdiv_ui (T, p);
      a = sp_inv (a, p, d);
      mpzspm->crt3[i] = a;
     
      /* crt1[i] = (P / p) mod modulus */
      mpz_init (mpzspm->crt1[i]);
      mpz_mod (mpzspm->crt1[i], T, modulus);

      /* crt4[i][j] = ((P / p[i]) mod modulus) mod p[j] */
      for (j = 0; j < mpzspm->sp_num; j++)
	mpzspm->crt4[j][i] = mpz_fdiv_ui (mpzspm->crt1[i], mpzspm->spm[j].sp);
      
      /* crt5[i] = (-P mod modulus) mod p */
      mpz_mod (T, P, modulus);
      mpz_sub (T, modulus, T);
      mpzspm->crt5[i] = mpz_fdiv_ui (T, p);
    }
  
  mpz_set_ui (T, 0);
  
  for (i = 0; i < mpzspm->sp_num + 2; i++)
    {
      mpz_mod (T, T, modulus);
      mpz_init_set (mpzspm->crt2[i], T);
      mpz_sub (T, T, P);
    }
  
  mpz_clear (P);
  mpz_clear (S);
  mpz_clear (T);

  return mpzspm;
}

void mpzspm_clear (mpzspm_t mpzspm)
{
  unsigned int i;

  for (i = 0; i < mpzspm->sp_num; i++)
    {
      mpz_clear (mpzspm->crt1[i]);
      free (mpzspm->crt4[i]);
    }

  for (i = 0; i < mpzspm->sp_num + 2; i++)
    mpz_clear (mpzspm->crt2[i]);
  
  free (mpzspm->crt1);
  free (mpzspm->crt2);
  free (mpzspm->crt3);
  free (mpzspm->crt4);
  free (mpzspm->crt5);
  
  mpz_clear (mpzspm->modulus);
  free (mpzspm->spm);
  free (mpzspm);
}

