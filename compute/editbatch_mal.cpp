/* File generated from [editbatch.c] by PICCO Mon Feb 22 20:23:38 2021
 */

#include <limits.h>

#include <float.h>

// extern void *memcpy(void*,const void*,unsigned int);

//# 1 "ort.onoff.defs"

extern "C" int ort_initialize(int *, char ***);
extern "C" void ort_finalize(int);

/* File generated from [editbatch.c] by PICCO Mon Feb 22 20:23:38 2021
 */

#include "smc-compute/SMC_Utils.h"

#include <gmp.h>

#include <omp.h>

SMC_Utils *__s;
int rep = 1;
mpz_t r;

int N = 100;

int p = 0;

int __original_main(int _argc_ignored, char **_argv_ignored)
{

  printf("Start computation \n");
  struct timeval start;
  struct timeval end;
  unsigned long timerp1;

  int i, j;

  mpz_t **A;
  A = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
  A[0] = (mpz_t *)malloc(sizeof(mpz_t) * N);
  A[1] = (mpz_t *)malloc(sizeof(mpz_t) * N);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
  {
    mpz_init(A[0][_picco_i]);
    mpz_init(A[1][_picco_i]);
  }

  mpz_t **B;
  B = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
  B[0] = (mpz_t *)malloc(sizeof(mpz_t) * N);
  B[1] = (mpz_t *)malloc(sizeof(mpz_t) * N);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
  {
    mpz_init(B[0][_picco_i]);
    mpz_init(B[1][_picco_i]);
  }
  mpz_t ***AB;
  AB = (mpz_t ***)malloc(sizeof(mpz_t **) * 2);
  AB[0] = (mpz_t **)malloc(sizeof(mpz_t *) * (N));
  AB[1] = (mpz_t **)malloc(sizeof(mpz_t *) * (N));
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
  {
    AB[0][_picco_i] = (mpz_t *)malloc(sizeof(mpz_t) * (N));
    AB[1][_picco_i] = (mpz_t *)malloc(sizeof(mpz_t) * (N));
    for (int _picco_j = 0; _picco_j < N; _picco_j++)
    {
      mpz_init(AB[0][_picco_i][_picco_j]);
      mpz_init(AB[1][_picco_i][_picco_j]);
    }
  }
  mpz_t **ABtemp;
  ABtemp = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
  ABtemp[0] = (mpz_t *)malloc(sizeof(mpz_t) * N * N);
  ABtemp[1] = (mpz_t *)malloc(sizeof(mpz_t) * N * N);
  for (int _picco_i = 0; _picco_i < N * N; _picco_i++)
  {
    mpz_init(ABtemp[0][_picco_i]);
    mpz_init(ABtemp[1][_picco_i]);
  }

  mpz_t **Atemp;
  Atemp = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
  Atemp[0] = (mpz_t *)malloc(sizeof(mpz_t) * N * N);
  Atemp[1] = (mpz_t *)malloc(sizeof(mpz_t) * N * N);
  for (int _picco_i = 0; _picco_i < N * N; _picco_i++)
  {
    mpz_init(Atemp[0][_picco_i]);
    mpz_init(Atemp[1][_picco_i]);
  }
  mpz_t **Btemp;
  Btemp = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
  Btemp[0] = (mpz_t *)malloc(sizeof(mpz_t) * N * N);
  Btemp[1] = (mpz_t *)malloc(sizeof(mpz_t) * N * N);
  for (int _picco_i = 0; _picco_i < N * N; _picco_i++)
  {
    mpz_init(Btemp[0][_picco_i]);
    mpz_init(Btemp[1][_picco_i]);
  }
  mpz_t **C1;
  C1 = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
  C1[0] = (mpz_t *)malloc(sizeof(mpz_t) * N);
  C1[1] = (mpz_t *)malloc(sizeof(mpz_t) * N);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
  {
    mpz_init(C1[0][_picco_i]);
    mpz_init(C1[1][_picco_i]);
  }

  mpz_t **C2;
  C2 = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
  C2[0] = (mpz_t *)malloc(sizeof(mpz_t) * N);
  C2[1] = (mpz_t *)malloc(sizeof(mpz_t) * N);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
  {
    mpz_init(C2[0][_picco_i]);
    mpz_init(C2[1][_picco_i]);
  }

  mpz_t **C3;
  C3 = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
  C3[0] = (mpz_t *)malloc(sizeof(mpz_t) * N);
  C3[1] = (mpz_t *)malloc(sizeof(mpz_t) * N);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
  {
    mpz_init(C3[0][_picco_i]);
    mpz_init(C3[1][_picco_i]);
  }
  mpz_t **CR;
  CR = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
  CR[0] = (mpz_t *)malloc(sizeof(mpz_t) * N);
  CR[1] = (mpz_t *)malloc(sizeof(mpz_t) * N);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
  {
    mpz_init(CR[0][_picco_i]);
    mpz_init(CR[1][_picco_i]);
  }
  mpz_t ***C;
  C = (mpz_t ***)malloc(sizeof(mpz_t **) * 2);
  C[0] = (mpz_t **)malloc(sizeof(mpz_t *) * (N + 1));
  C[1] = (mpz_t **)malloc(sizeof(mpz_t *) * (N + 1));
  for (int _picco_i = 0; _picco_i < N + 1; _picco_i++)
  {
    C[0][_picco_i] = (mpz_t *)malloc(sizeof(mpz_t) * (N + 1));
    C[1][_picco_i] = (mpz_t *)malloc(sizeof(mpz_t) * (N + 1));
    for (int _picco_j = 0; _picco_j < N + 1; _picco_j++)
    {
      mpz_init(C[0][_picco_i][_picco_j]);
      mpz_init(C[1][_picco_i][_picco_j]);
    }
  }

  mpz_t *a = (mpz_t *)malloc(sizeof(mpz_t) * (2));
  mpz_init(a[0]);
  mpz_init(a[1]);
  mpz_t *b = (mpz_t *)malloc(sizeof(mpz_t) * (2));
  mpz_init(b[0]);
  mpz_init(b[1]);

  mpz_t tmp;
  mpz_init(tmp);

  mpz_t *dtmp = (mpz_t *)malloc(sizeof(mpz_t) * (2));
  mpz_init(dtmp[0]);
  mpz_init(dtmp[1]);

  //__s->smc_input(1, &a, "int", -1);
  //__s->smc_input(1, &b, "int", -1);
  printf("input a\n");
  __s->smc_input_mal(1, a, 1, 8, "int", -1);
  printf("input b\n");
  __s->smc_input_mal(1, b, 1, 8, "int", -1);

  i = 0;
  for (; i < N;)
  {
    {
      __s->smc_set(a[0], A[0][i], 8, 8, "int", -1);
      __s->smc_set(a[1], A[1][i], 8, 8, "int", -1);

      __s->smc_set(b[0], B[0][i], 8, 8, "int", -1);
      __s->smc_set(b[1], B[1][i], 8, 8, "int", -1);
    }
    i++;
  }
  gettimeofday(&start, NULL); // start timer here
  for (int ri = 0; ri < rep; ri++)
  {
    printf("%d th start \n", ri);
    p = 0;

    i = 0;
    for (; i < N;)
    {
      {
        j = 0;
        for (; j < N;)
        {
          {
            __s->smc_set(A[0][i], Atemp[0][p], 8, 8, "int", -1);
            __s->smc_set(A[1][i], Atemp[1][p], 8, 8, "int", -1);

            __s->smc_set(B[0][j], Btemp[0][p], 8, 8, "int", -1);
            __s->smc_set(B[1][j], Btemp[1][p], 8, 8, "int", -1);

            p++;
          }
          j++;
        }
      }
      i++;
    }

    __s->smc_eqeq_mal(Atemp, Btemp, 8, 8, ABtemp, 1, N * N, "int", -1);

    p = 0;

    i = 0;
    for (; i < N;)
    {
      {
        j = 0;
        for (; j < N;)
        {
          {
            __s->smc_set(ABtemp[0][p], AB[0][i][j], 1, 1, "int", -1);
            __s->smc_set(ABtemp[1][p], AB[1][i][j], 1, 1, "int", -1);

            p++;
          }
          j++;
        }
      }
      i++;
    }

    i = 1;
    for (; i <= N;)
    {
      {
        __s->smc_set(i, C[0][i][0], -1, 7, "int", -1);
        __s->smc_mult(r, i, tmp, 8, 7, 8, "int", -1);
        __s->smc_set(tmp, C[1][i][0], -1, 7, "int", -1);

        __s->smc_set(i, C[0][0][i], -1, 7, "int", -1);
        __s->smc_set(tmp, C[1][0][i], -1, 7, "int", -1);
      }
      i++;
    }
    //__s->smc_sub(1, AB[0][0], _picco_tmp1, -1, 1, 1, "int", -1);
    mpz_set(dtmp[0], AB[0][0][0]);
    mpz_set(dtmp[1], AB[1][0][0]);
    __s->smc_sub_mal(1, dtmp, dtmp, 8, 1, 1, "int", -1);

    __s->smc_set(dtmp[0], C[0][1][1], 1, 7, "int", -1);
    __s->smc_set(dtmp[1], C[1][1][1], 1, 7, "int", -1);

    j = 2;
    for (; j <= N;)
    {
      {
        i = 1;
        for (; i <= j;)
        {
          {
            mpz_set(dtmp[0], C[0][j - i][i - 1]);
            mpz_set(dtmp[1], C[1][j - i][i - 1]);
            __s->smc_add_mal(1, dtmp, dtmp, 7, 7, 7, "int", -1);
            //__s->smc_add(C[j - i][i - 1], 1, _picco_tmp1, 7, -1, 7, "int", -1)
            __s->smc_sub(dtmp[0], AB[0][j - i][i - 1], dtmp[0], 7, 1, 7, "int", -1);
            __s->smc_sub(dtmp[1], AB[1][j - i][i - 1], dtmp[1], 7, 1, 7, "int", -1);

            __s->smc_set(dtmp[0], C[0][j - i + 1][i], 7, 7, "int", -1);
            __s->smc_set(dtmp[1], C[1][j - i + 1][i], 7, 7, "int", -1);

            __s->smc_set(C[0][j - i + 1][i], C1[0][i - 1], 7, 7, "int", -1);
            __s->smc_set(C[1][j - i + 1][i], C1[1][i - 1], 7, 7, "int", -1);

            mpz_set(dtmp[0], C[0][j - i][i]);
            mpz_set(dtmp[1], C[1][j - i][i]);
            __s->smc_add_mal(1, dtmp, dtmp, 7, 7, 7, "int", -1);
            //__s->smc_add(C[j - i][i], 1, _picco_tmp1, 7, -1, 7, "int", -1);

            __s->smc_set(dtmp[0], C2[0][i - 1], 7, 7, "int", -1);
            __s->smc_set(dtmp[1], C2[1][i - 1], 7, 7, "int", -1);

            mpz_set(dtmp[0], C[0][j - i + 1][i - 1]);
            mpz_set(dtmp[1], C[1][j - i + 1][i - 1]);
            __s->smc_add_mal(1, dtmp, dtmp, 7, 7, 7, "int", -1);
            //__s->smc_add(C[j - i + 1][i - 1], 1, _picco_tmp1, 7, -1, 7, "int", -1);

            __s->smc_set(dtmp[0], C3[0][i - 1], 7, 7, "int", -1);
            __s->smc_set(dtmp[1], C3[1][i - 1], 7, 7, "int", -1);
          }
          i++;
        }
        __s->smc_leq_mal(C2, C1, 7, 7, CR, 1, j, "int", -1);

        __s->smc_sub(C2[0], C1[0], 7, 7, C2[0], 7, j, "int", -1);
        __s->smc_sub(C2[1], C1[1], 7, 7, C2[1], 7, j, "int", -1);

        __s->smc_mult_mal(CR, C2, 1, 7, C2, 7, j, "int", -1);

        __s->smc_add_mal(C1, C2, 7, 7, C1, 7, j, "int", -1);

        __s->smc_leq_mal(C3, C1, 7, 7, CR, 1, j, "int", -1);

        __s->smc_sub(C3[0], C1[0], 7, 7, C3[0], 7, j, "int", -1);
        __s->smc_sub(C3[1], C1[1], 7, 7, C3[1], 7, j, "int", -1);

        __s->smc_mult_mal(CR, C3, 1, 7, C3, 7, j, "int", -1);

        __s->smc_add_mal(C1, C3, 7, 7, C1, 7, j, "int", -1);

        i = 1;
        for (; i <= j;)
        {
          {
            __s->smc_set(C1[0][i - 1], C[0][j - i + 1][i], 7, 7, "int", -1);
            __s->smc_set(C1[1][i - 1], C[1][j - i + 1][i], 7, 7, "int", -1);
          }
          i++;
        }
      }
      j++;
    }

    j = N - 1;
    for (; j >= 1;)
    {
      {
        i = 0;
        for (; i < j;)
        {
          {
            mpz_set(dtmp[0], C[0][N - i - 1][N - j + i]);
            mpz_set(dtmp[1], C[1][N - i - 1][N - j + i]);
            __s->smc_add_mal(1, dtmp, dtmp, 7, 7, 7, "int", -1);
            //__s->smc_add(C[N - i - 1][N - j + i], 1, _picco_tmp1, 7, -1, 7, "int", -1);

            __s->smc_sub(dtmp[0], AB[0][N - i - 1][N - j + i], dtmp[0], 7, 1, 7, "int", -1);
            __s->smc_sub(dtmp[1], AB[1][N - i - 1][N - j + i], dtmp[1], 7, 1, 7, "int", -1);

            __s->smc_set(dtmp[0], C[0][N - i][N - j + i + 1], 7, 7, "int", -1);
            __s->smc_set(dtmp[1], C[1][N - i][N - j + i + 1], 7, 7, "int", -1);

            __s->smc_set(C[0][N - i][N - j + i + 1], C1[0][i], 7, 7, "int", -1);
            __s->smc_set(C[1][N - i][N - j + i + 1], C1[1][i], 7, 7, "int", -1);

            mpz_set(dtmp[0], C[0][N - i - 1][N - j + i + 1]);
            mpz_set(dtmp[1], C[1][N - i - 1][N - j + i + 1]);
            __s->smc_add_mal(1, dtmp, dtmp, 7, 7, 7, "int", -1);
            //__s->smc_add(C[N - i - 1][N - j + i + 1], 1, _picco_tmp1, 7, -1, 7, "int", -1);
            __s->smc_set(dtmp[0], C2[0][i], 7, 7, "int", -1);
            __s->smc_set(dtmp[1], C2[1][i], 7, 7, "int", -1);

            mpz_set(dtmp[0], C[0][N - i][N - j + i]);
            mpz_set(dtmp[1], C[1][N - i][N - j + i]);
            __s->smc_add_mal(1, dtmp, dtmp, 7, 7, 7, "int", -1);
            //__s->smc_add(C[N - i][N - j + i], 1, _picco_tmp1, 7, -1, 7, "int", -1);
            __s->smc_set(dtmp[0], C3[0][i], 7, 7, "int", -1);
            __s->smc_set(dtmp[1], C3[1][i], 7, 7, "int", -1);
          }
          i++;
        }
        __s->smc_leq_mal(C2, C1, 7, 7, CR, 1, j, "int", -1);

        __s->smc_sub(C2[0], C1[0], 7, 7, C2[0], 7, j, "int", -1);
        __s->smc_sub(C2[1], C1[1], 7, 7, C2[1], 7, j, "int", -1);

        __s->smc_mult_mal(CR, C2, 1, 7, C2, 7, j, "int", -1);

        __s->smc_add_mal(C1, C2, 7, 7, C1, 7, j, "int", -1);

        __s->smc_leq_mal(C3, C1, 7, 7, CR, 1, j, "int", -1);

        __s->smc_sub(C3[0], C1[0], 7, 7, C3[0], 7, j, "int", -1);
        __s->smc_sub(C3[1], C1[1], 7, 7, C3[1], 7, j, "int", -1);

        __s->smc_mult_mal(CR, C3, 1, 7, C3, 7, j, "int", -1);

        __s->smc_add_mal(C1, C3, 7, 7, C1, 7, j, "int", -1);

        i = 0;
        for (; i < j;)
        {
          {
            //__s->smc_set(C1[i], C[N-i][N-j+i+1], 7, 7, "int", -1);
            __s->smc_set(C1[0][i], C[0][N - i][N - j + i + 1], 7, 7, "int", -1);
            __s->smc_set(C1[1][i], C[1][N - i][N - j + i + 1], 7, 7, "int", -1);
          }
          i++;
        }
      }
      j--;
    }
    __s->smc_verify();
  }
  gettimeofday(&end, NULL); // stop timer here p1
  timerp1 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  printf("runtime for  edit distance of size %d = %ld us\n", N, timerp1);
  __s->smc_output(1, &C[0][N][N], "int", -1);

  mpz_clear(b[0]);
  mpz_clear(b[1]);
  mpz_clear(a[0]);
  mpz_clear(a[1]);

  for (int k = 0; k < 2; k++)
  {
    for (int _picco_i = 0; _picco_i < N + 1; _picco_i++)
    {
      for (int _picco_j = 0; _picco_j < N + 1; _picco_j++)
        mpz_clear(C[k][_picco_i][_picco_j]);
      free(C[k][_picco_i]);
    }
    free(C[k]);
  }
  free(C);

  for (int _picco_i = 0; _picco_i < 2; _picco_i++)
  {
    for (int _picco_j = 0; _picco_j < N; _picco_j++)
    {
      mpz_clear(C1[_picco_i][_picco_j]);
      mpz_clear(C2[_picco_i][_picco_j]);
      mpz_clear(C3[_picco_i][_picco_j]);
      mpz_clear(CR[_picco_i][_picco_j]);
      mpz_clear(A[_picco_i][_picco_j]);
      mpz_clear(B[_picco_i][_picco_j]);
    }
    free(C1[_picco_i]);
    free(C2[_picco_i]);
    free(C3[_picco_i]);
    free(CR[_picco_i]);
    free(A[_picco_i]);
    free(B[_picco_i]);
  }
  free(C1);
  free(C2);
  free(C3);
  free(CR);
  free(A);
  free(B);

  for (int _picco_i = 0; _picco_i < 2; _picco_i++)
  {
    for (int _picco_j = 0; _picco_j < N * N; _picco_j++)
    {
      mpz_clear(Btemp[_picco_i][_picco_j]);
      mpz_clear(Atemp[_picco_i][_picco_j]);
      mpz_clear(ABtemp[_picco_i][_picco_j]);
    }
    free(Btemp[_picco_i]);
    free(Atemp[_picco_i]);
    free(ABtemp[_picco_i]);
  }
  free(Btemp);
  free(Atemp);
  free(ABtemp);

  for (int k = 0; k < 2; k++)
  {
    for (int _picco_i = 0; _picco_i < N; _picco_i++)
    {
      for (int _picco_j = 0; _picco_j < N; _picco_j++)
        mpz_clear(AB[k][_picco_i][_picco_j]);
      free(AB[k][_picco_i]);
    }
    free(AB[k]);
  }
  free(AB);

  return (0);
}
/* smc-compiler generated main() */
int main(int argc, char **argv)
{
  if (argc < 10)
  {
    fprintf(stderr, "Incorrect input parameters\n");
    fprintf(stderr, "Usage: <id> <runtime-config> <privatekey-filename> <numb\
er-of-input-parties> <number-of-output-parties> <input-share> <output> <size\
> <rep>\n");
    exit(1);
  }
  std::string IO_files[atoi(argv[4]) + atoi(argv[5])];
  for (int i = 0; i < 2; i++)
    IO_files[i] = argv[6 + i];
  N = atoi(argv[8]);
  rep = atoi(argv[9]);
  __s = new SMC_Utils(atoi(argv[1]), argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), IO_files, 3, 1, 129, "340282366920938463463374607431768211507", 1);

  struct timeval tv1;
  struct timeval tv2;
  int _xval = 0;
  __s->smc_init_mal();
  mpz_init(r);
  __s->smc_getr_mal(r);
  gettimeofday(&tv1, NULL);
  _xval = (int)__original_main(argc, argv);
  gettimeofday(&tv2, NULL);
  __s->smc_clean_mal();
  std::cout << "Time: " << __s->time_diff(&tv1, &tv2) << std::endl;
  return (_xval);
}
