/* File generated from [editbatch.c] by PICCO Mon Feb 22 20:23:38 2021
 */

#include <limits.h>

#include <float.h>

//extern void *memcpy(void*,const void*,unsigned int);

//# 1 "ort.onoff.defs"

extern  "C" int   ort_initialize(int*, char***);
 extern "C" void  ort_finalize(int);


/* File generated from [editbatch.c] by PICCO Mon Feb 22 20:23:38 2021
 */

#include "smc-compute/SMC_Utils.h"

#include <gmp.h>

#include <omp.h>


SMC_Utils *__s;

int rep = 1;
int N = 100;

int p = 0;


int  __original_main(int _argc_ignored, char **_argv_ignored)
{
  struct timeval start;
  struct timeval end;
  unsigned long timerp1;
  
  mpz_t _picco_tmp1, _picco_tmp2;
  mpz_init(_picco_tmp1);
  mpz_init(_picco_tmp2);

  mpz_t* _picco_ftmp1 = (mpz_t*)malloc(sizeof(mpz_t) * 4);
  for(int _picco_j = 0; _picco_j < 4; _picco_j++)
    mpz_init(_picco_ftmp1[_picco_j]);
  void* _picco_temp_;
 int i, j;

  mpz_t* A; 
  A = (mpz_t*)malloc(sizeof(mpz_t) * (N));
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_init(A[_picco_i]);
  mpz_t* B; 
  B = (mpz_t*)malloc(sizeof(mpz_t) * (N));
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_init(B[_picco_i]);

  mpz_t** AB; 
  AB = (mpz_t**)malloc(sizeof(mpz_t*) * (N));
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
    {
      AB[_picco_i] = (mpz_t*)malloc(sizeof(mpz_t) * (N));
      for (int _picco_j = 0; _picco_j < N; _picco_j++)
            mpz_init(AB[_picco_i][_picco_j]);
    }

  mpz_t* ABtemp; 
  ABtemp = (mpz_t*)malloc(sizeof(mpz_t) * (N * N));
  for (int _picco_i = 0; _picco_i < N * N; _picco_i++)
        mpz_init(ABtemp[_picco_i]);

  mpz_t* Atemp; 
  Atemp = (mpz_t*)malloc(sizeof(mpz_t) * (N * N));
  for (int _picco_i = 0; _picco_i < N * N; _picco_i++)
        mpz_init(Atemp[_picco_i]);

  mpz_t* Btemp; 
  Btemp = (mpz_t*)malloc(sizeof(mpz_t) * (N * N));
  for (int _picco_i = 0; _picco_i < N * N; _picco_i++)
        mpz_init(Btemp[_picco_i]);

  mpz_t* C1; 
  C1 = (mpz_t*)malloc(sizeof(mpz_t) * (N));
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_init(C1[_picco_i]);

  mpz_t* C2; 
  C2 = (mpz_t*)malloc(sizeof(mpz_t) * (N));
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_init(C2[_picco_i]);

  mpz_t* C3; 
  C3 = (mpz_t*)malloc(sizeof(mpz_t) * (N));
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_init(C3[_picco_i]);

  mpz_t* CR; 
  CR = (mpz_t*)malloc(sizeof(mpz_t) * (N));
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_init(CR[_picco_i]);

  mpz_t** C; 
  C = (mpz_t**)malloc(sizeof(mpz_t*) * (N + 1));
  for (int _picco_i = 0; _picco_i < N + 1; _picco_i++)
    {
      C[_picco_i] = (mpz_t*)malloc(sizeof(mpz_t) * (N + 1));
      for (int _picco_j = 0; _picco_j < N + 1; _picco_j++)
            mpz_init(C[_picco_i][_picco_j]);
    }

  mpz_t a;
  mpz_init(a);
  mpz_t b;
  mpz_init(b);

  __s->smc_input(1, &a, "int", -1);

  __s->smc_input(1, &b, "int", -1);

  i = 0;
  for ( ;i < N; )
  {
    {
            __s->smc_set(a, A[i], 8, 8, "int", -1);

            __s->smc_set(b, B[i], 8, 8, "int", -1);
    }
    i++;
  }

  gettimeofday(&start,NULL); //start timer here                                                 
  for(int ri = 0; ri < rep; ri++){
  p = 0;

  i = 0;
  for ( ;i < N; )
  {
    {
      j = 0;
      for ( ;j < N; )
      {
        {
                    __s->smc_set(A[i], Atemp[p], 8, 8, "int", -1);

                    __s->smc_set(B[j], Btemp[p], 8, 8, "int", -1);

          p++;
        }
        j++;
      }
    }
    i++;
  }

  __s->smc_eqeq(Atemp, Btemp, 8, 8, ABtemp, 1, N * N, "int", -1);

  p = 0;

  i = 0;
  for ( ;i < N; )
  {
    {
      j = 0;
      for ( ;j < N; )
      {
        {
                    __s->smc_set(ABtemp[p], AB[i][j], 1, 1, "int", -1);

          p++;
        }
        j++;
      }
    }
    i++;
  }

  i = 1;
  for ( ;i <= N; )
  {
    {
      __s->smc_set(i, C[i][0], -1, 7, "int", -1);

      __s->smc_set(i, C[0][i], -1, 7, "int", -1);
    }
    i++;
  }

  __s->smc_sub(1, AB[0][0], _picco_tmp1, -1, 1, 1, "int", -1);
  __s->smc_set(_picco_tmp1, C[1][1], 1, 7, "int", -1);

  j = 2;
  for ( ;j <= N; )
  {
    {
      i = 1;
      for ( ;i <= j; )
      {
        {
          __s->smc_add(C[j - i][i - 1], 1, _picco_tmp1, 7, -1, 7, "int", -1);
          __s->smc_sub(_picco_tmp1, AB[j - i][i - 1], _picco_tmp1, 7, 1, 7, "int", -1);
          __s->smc_set(_picco_tmp1, C[j - i + 1][i], 7, 7, "int", -1);

                    __s->smc_set(C[j - i + 1][i], C1[i - 1], 7, 7, "int", -1);

          __s->smc_add(C[j - i][i], 1, _picco_tmp1, 7, -1, 7, "int", -1);
          __s->smc_set(_picco_tmp1, C2[i - 1], 7, 7, "int", -1);

          __s->smc_add(C[j - i + 1][i - 1], 1, _picco_tmp1, 7, -1, 7, "int", -1);
          __s->smc_set(_picco_tmp1, C3[i - 1], 7, 7, "int", -1);
        }
        i++;
      }

      __s->smc_lt(C2, C1, 7, 7, CR, 1, j, "int", -1);

      __s->smc_sub(C2, C1, 7, 7, C2, 7, j, "int", -1);

      __s->smc_mult(CR, C2, 1, 7, C2, 7, j, "int", -1);

      __s->smc_add(C1, C2, 7, 7, C1, 7, j, "int", -1);

      __s->smc_lt(C3, C1, 7, 7, CR, 1, j, "int", -1);

      __s->smc_sub(C3, C1, 7, 7, C3, 7, j, "int", -1);

      __s->smc_mult(CR, C3, 1, 7, C3, 7, j, "int", -1);

      __s->smc_add(C1, C3, 7, 7, C1, 7, j, "int", -1);

      i = 1;
      for ( ;i <= j; )
      {
        {
                    __s->smc_set(C1[i - 1], C[j - i + 1][i], 7, 7, "int", -1);
        }
        i++;
      }
    }
    j++;
  }

  j = N - 1;
  for ( ;j >= 1; )
  {
    {
      i = 0;
      for ( ;i < j; )
      {
        {
          __s->smc_add(C[N - i - 1][N - j + i], 1, _picco_tmp1, 7, -1, 7, "int", -1);
          __s->smc_sub(_picco_tmp1, AB[N - i - 1][N - j + i], _picco_tmp1, 7, 1, 7, "int", -1);
          __s->smc_set(_picco_tmp1, C[N - i][N - j + i + 1], 7, 7, "int", -1);

                    __s->smc_set(C[N - i][N - j + i + 1], C1[i], 7, 7, "int", -1);

          __s->smc_add(C[N - i - 1][N - j + i + 1], 1, _picco_tmp1, 7, -1, 7, "int", -1);
          __s->smc_set(_picco_tmp1, C2[i], 7, 7, "int", -1);

          __s->smc_add(C[N - i][N - j + i], 1, _picco_tmp1, 7, -1, 7, "int", -1);
          __s->smc_set(_picco_tmp1, C3[i], 7, 7, "int", -1);
        }
        i++;
      }

      __s->smc_lt(C2, C1, 7, 7, CR, 1, j, "int", -1);

      __s->smc_sub(C2, C1, 7, 7, C2, 7, j, "int", -1);

      __s->smc_mult(CR, C2, 1, 7, C2, 7, j, "int", -1);

      __s->smc_add(C1, C2, 7, 7, C1, 7, j, "int", -1);

      __s->smc_lt(C3, C1, 7, 7, CR, 1, j, "int", -1);

      __s->smc_sub(C3, C1, 7, 7, C3, 7, j, "int", -1);

      __s->smc_mult(CR, C3, 1, 7, C3, 7, j, "int", -1);

      __s->smc_add(C1, C3, 7, 7, C1, 7, j, "int", -1);

      i = 1;
      for ( ;i <= j; )
      {
        {
                    __s->smc_set(C1[i - 1], C[j - i + 1][i], 7, 7, "int", -1);
        }
        i++;
      }
    }
    j--;
  }
  }
  gettimeofday(&end,NULL);//stop timer here p1                                                    
  timerp1 = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
  printf("runtime for  edit distance of size %d = %ld us\n", N,timerp1);
  __s->smc_output(1, &C[N][N], "int", -1);

    mpz_clear(b);
  mpz_clear(a);
  for (int _picco_i = 0; _picco_i < N + 1; _picco_i++)
    {
            for (int _picco_j = 0; _picco_j < N + 1; _picco_j++)
              mpz_clear(C[_picco_i][_picco_j]);
            free(C[_picco_i]);
      }
  free(C);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_clear(CR[_picco_i]);
  free(CR);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_clear(C3[_picco_i]);
  free(C3);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_clear(C2[_picco_i]);
  free(C2);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_clear(C1[_picco_i]);
  free(C1);
  for (int _picco_i = 0; _picco_i < N * N; _picco_i++)
        mpz_clear(Btemp[_picco_i]);
  free(Btemp);
  for (int _picco_i = 0; _picco_i < N * N; _picco_i++)
        mpz_clear(Atemp[_picco_i]);
  free(Atemp);
  for (int _picco_i = 0; _picco_i < N * N; _picco_i++)
        mpz_clear(ABtemp[_picco_i]);
  free(ABtemp);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
    {
            for (int _picco_j = 0; _picco_j < N; _picco_j++)
              mpz_clear(AB[_picco_i][_picco_j]);
            free(AB[_picco_i]);
      }
  free(AB);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_clear(B[_picco_i]);
  free(B);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_clear(A[_picco_i]);
  free(A);
  mpz_clear(_picco_tmp1);
  mpz_clear(_picco_tmp2);
  for(int _picco_j = 0; _picco_j < 4; _picco_j++)
    mpz_clear(_picco_ftmp1[_picco_j]);
  free(_picco_ftmp1);
  return (0);
}


/* smc-compiler generated main() */
int main(int argc, char **argv)
{
  if(argc < 10){
    fprintf(stderr,"Incorrect input parameters\n");
    fprintf(stderr,"Usage: <id> <runtime-config> <privatekey-filename> <number-of-input-parties> <number-of-output-parties> <input-share> <output> <size> <rep>\n");
    exit(1);
  }
  std::string IO_files[atoi(argv[4]) + atoi(argv[5])];
  for(int i = 0; i < 2; i++)
    IO_files[i] = argv[6+i];
  N = atoi(argv[8]);
  rep = atoi(argv[9]);

__s = new SMC_Utils(atoi(argv[1]), argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), IO_files, 3, 1, 57, "72057594037928111", 1);

struct timeval tv1;
struct timeval tv2;  int _xval = 0;

  gettimeofday(&tv1,NULL);
  _xval = (int) __original_main(argc, argv);
  gettimeofday(&tv2, NULL);
  std::cout << "Time: " << __s->time_diff(&tv1,&tv2) << std::endl;
  return (_xval);
}

