/* File generated from [xor.c] by PICCO Tue Feb 23 23:02:42 2021
 */

#include <limits.h>

#include <float.h>

//extern void *memcpy(void*,const void*,unsigned int);

//# 1 "ort.onoff.defs"

extern  "C" int   ort_initialize(int*, char***);
 extern "C" void  ort_finalize(int);


/* File generated from [xor.c] by PICCO Tue Feb 23 23:02:42 2021
 */

#include "smc-compute/SMC_Utils.h"

#include <gmp.h>

#include <omp.h>


SMC_Utils *__s;

int rep = 0;

int N = 100;


int  __original_main(int _argc_ignored, char **_argv_ignored)
{
  printf("Start computation \n");
  struct timeval start;
  struct timeval end;
  unsigned long timerp1;
  
  mpz_t _picco_tmp1;
  mpz_init(_picco_tmp1);

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

  mpz_t* AB; 
  AB = (mpz_t*)malloc(sizeof(mpz_t) * (N));
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_init(AB[_picco_i]);

  mpz_t* Temp; 
  Temp = (mpz_t*)malloc(sizeof(mpz_t) * (N));
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_init(Temp[_picco_i]);

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
            __s->smc_set(a, A[i], 1, 1, "int", -1);

            __s->smc_set(b, B[i], 1, 1, "int", -1);
    }
    i++;
  }

  gettimeofday(&start,NULL); //start timer here                                      
  for(int ri = 0; ri < rep; ri++){

  __s->smc_add(A, B, 1, 1, AB, 1, N, "int", -1);

  __s->smc_mult(A, B, 1, 1, Temp, 1, N, "int", -1);

  i = 0;
  for(; i < N; ){
    __s->smc_mult(Temp[i], 2, Temp[i], 2, 2, 2,"int", -1);
    i++;
  }

  __s->smc_sub(AB, Temp, 1, 1, AB, 1, N, "int", -1);

  }
  gettimeofday(&end,NULL);//stop timer here p1                                       
  timerp1 = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
  printf("runtime for  hamming distance of size %d = %ld us\n", N,timerp1);

  __s->smc_output(1, &AB[0], "int", -1);

    mpz_clear(b);
  mpz_clear(a);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_clear(Temp[_picco_i]);
  free(Temp);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_clear(AB[_picco_i]);
  free(AB);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_clear(B[_picco_i]);
  free(B);
  for (int _picco_i = 0; _picco_i < N; _picco_i++)
        mpz_clear(A[_picco_i]);
  free(A);
  mpz_clear(_picco_tmp1);
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

__s = new SMC_Utils(atoi(argv[1]), argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), IO_files, 3, 1, 3, "7", 1);

struct timeval tv1;
struct timeval tv2;  int _xval = 0;

  gettimeofday(&tv1,NULL);
  _xval = (int) __original_main(argc, argv);
  gettimeofday(&tv2, NULL);
  std::cout << "Time: " << __s->time_diff(&tv1,&tv2) << std::endl;
  return (_xval);
}

