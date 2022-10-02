/* File generated from [hamming.c] by PICCO Tue Feb 23 16:14:53 2021
 */

#include <limits.h>

#include <float.h>

// extern void *memcpy(void*,const void*,unsigned int);

//# 1 "ort.onoff.defs"

extern "C" int ort_initialize(int *, char ***);
extern "C" void ort_finalize(int);

/* File generated from [hamming.c] by PICCO Tue Feb 23 16:14:53 2021
 */

#include "smc-compute/SMC_Utils.h"

#include <gmp.h>

#include <omp.h>

SMC_Utils *__s;
int rep = 0;

int N = 100;
int __original_main(int _argc_ignored, char **_argv_ignored)
{
  printf("Start computation \n");
  struct timeval start;
  struct timeval end;
  unsigned long timerp1;

  int final = 0;

  mpz_t *_picco_tmp1 = (mpz_t *)malloc(sizeof(mpz_t) * (2));
  mpz_init(_picco_tmp1[0]);
  mpz_init(_picco_tmp1[1]);

  mpz_t *_picco_ftmp1 = (mpz_t *)malloc(sizeof(mpz_t) * 4);
  for (int _picco_j = 0; _picco_j < 4; _picco_j++)
    mpz_init(_picco_ftmp1[_picco_j]);
  void *_picco_temp_;
  int i, j;

  mpz_t **A;
  A = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
  for (int _picco_i = 0; _picco_i < 2; _picco_i++)
  {
    A[_picco_i] = (mpz_t *)malloc(sizeof(mpz_t) * (N));
    for (int _picco_j = 0; _picco_j < N; _picco_j++)
    {
      mpz_init(A[_picco_i][_picco_j]);
    }
  }

  mpz_t **B;
  B = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
  for (int _picco_i = 0; _picco_i < 2; _picco_i++)
  {
    B[_picco_i] = (mpz_t *)malloc(sizeof(mpz_t) * (N));
    for (int _picco_j = 0; _picco_j < N; _picco_j++)
    {
      mpz_init(B[_picco_i][_picco_j]);
    }
  }

  mpz_t **C;
  C = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
  for (int _picco_i = 0; _picco_i < 2; _picco_i++)
  {
    C[_picco_i] = (mpz_t *)malloc(sizeof(mpz_t) * (N));
    for (int _picco_j = 0; _picco_j < N; _picco_j++)
    {
      mpz_init(C[_picco_i][_picco_j]);
    }
  }
  mpz_t a[2];
  mpz_init(a[0]);
  mpz_init(a[1]);

  mpz_t b[2];
  mpz_init(b[0]);
  mpz_init(b[1]);

  mpz_t res[2];
  mpz_init(res[0]);
  mpz_init(res[1]);

  printf("input a\n");
  __s->smc_input_mal(1, a, 1, 8, "int", -1);
  printf("input b\n");
  __s->smc_input_mal(1, b, 1, 8, "int", -1);

  i = 0;
  for (; i < N;)
  {
    {
      __s->smc_set(a[0], A[0][i], 32, 32, "int", -1);
      __s->smc_set(a[1], A[1][i], 32, 32, "int", -1);

      __s->smc_set(b[0], B[0][i], 32, 32, "int", -1);
      __s->smc_set(b[1], B[1][i], 32, 32, "int", -1);
    }
    i++;
  }

  gettimeofday(&start, NULL); // start timer here
  for (int ri = 0; ri < rep; ri++)
  {
    printf("%d th start\n", ri);
    mpz_set_ui(res[0], 0);
    mpz_set_ui(res[1], 0);
    __s->smc_eqeq_mal(A, B, 8, 8, C, 1, N, "int", -1);

    i = 0;
    for (; i < N;)
    {
      {
        mpz_set(_picco_tmp1[0], C[0][i]);
        mpz_set(_picco_tmp1[1], C[1][i]);
        __s->smc_sub_mal(1, _picco_tmp1, _picco_tmp1, 7, 1, 7, "int", -1);
        __s->smc_add(res[0], _picco_tmp1[0], _picco_tmp1[0], 7, 1, 7, "int", -1);
        __s->smc_add(res[1], _picco_tmp1[1], _picco_tmp1[1], 7, 1, 7, "int", -1);

        __s->smc_set(_picco_tmp1[0], res[0], 7, 7, "int", -1);
        __s->smc_set(_picco_tmp1[1], res[1], 7, 7, "int", -1);
      }
      i++;
    }
    __s->smc_verify();
  }
  gettimeofday(&end, NULL); // stop timer here p1
  timerp1 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  printf("runtime for  hamming distance of size %d = %ld us\n", N, timerp1);

  //__s->smc_output(1, &res, "int", -1);
  final = __s->smc_open(res[0], -1);
  printf("final res is %d \n", final);

  mpz_clear(b[0]);
  mpz_clear(b[1]);
  mpz_clear(a[0]);
  mpz_clear(a[1]);
  mpz_clear(res[0]);
  mpz_clear(res[1]);

  mpz_clear(_picco_tmp1[0]);
  mpz_clear(_picco_tmp1[1]);

  for (int _picco_i = 0; _picco_i < 2; _picco_i++)
  {
    for (int _picco_j = 0; _picco_j < N; _picco_j++)
    {
      mpz_clear(C[_picco_i][_picco_j]);
      mpz_clear(A[_picco_i][_picco_j]);
      mpz_clear(B[_picco_i][_picco_j]);
    }
    free(C[_picco_i]);
    free(A[_picco_i]);
    free(B[_picco_i]);
  }
  free(C);
  free(A);
  free(B);

  for (int _picco_j = 0; _picco_j < 4; _picco_j++)
    mpz_clear(_picco_ftmp1[_picco_j]);
  free(_picco_ftmp1);
  return (0);
}

/* smc-compiler generated main() */
int main(int argc, char **argv)
{
  if (argc < 10)
  {
    fprintf(stderr, "Incorrect input parameters\n");
    fprintf(stderr, "Usage: <id> <runtime-config> <privatekey-filename> <number-of-input-parties> <number-of-output-parties> <input-share> <output> <size> <rep>\n");
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
  gettimeofday(&tv1, NULL);
  _xval = (int)__original_main(argc, argv);
  gettimeofday(&tv2, NULL);
  __s->smc_clean_mal();
  std::cout << "Time: " << __s->time_diff(&tv1, &tv2) << std::endl;
  return (_xval);
}
