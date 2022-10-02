/* File generated from [mult-add_Batch.c] by PICCO Tue Nov 10 19:43:49 2020
 */

#include <limits.h>

#include <float.h>

//extern void *memcpy(void*,const void*,unsigned int);

//# 1 "ort.onoff.defs"

extern  "C" int   ort_initialize(int*, char***);
 extern "C" void  ort_finalize(int);


/* File generated from [mult-add_Batch.c] by PICCO Tue Nov 10 19:43:49 2020
 */

#include "smc-compute/SMC_Utils.h"

#include <gmp.h>

#include <omp.h>


SMC_Utils *__s;


int S = 20;
int rep = 1;

int  __original_main(int _argc_ignored, char **_argv_ignored)
{
    struct timeval start;
  struct timeval end;
  unsigned long timerp1;

  mpz_t _picco_tmp1;
  mpz_init(_picco_tmp1);

  mpz_t* _picco_ftmp1 = (mpz_t*)malloc(sizeof(mpz_t) * 4);
  for(int _picco_j = 0; _picco_j < 4; _picco_j++)
    mpz_init(_picco_ftmp1[_picco_j]);
  void* _picco_temp_;
 mpz_t* a; 
  a = (mpz_t*)malloc(sizeof(mpz_t) * (S));
  for (int _picco_i = 0; _picco_i < S; _picco_i++)
        mpz_init(a[_picco_i]);
  mpz_t* b; 
  b = (mpz_t*)malloc(sizeof(mpz_t) * (S));
  for (int _picco_i = 0; _picco_i < S; _picco_i++)
        mpz_init(b[_picco_i]);

  mpz_t input;
  mpz_init(input);

  __s->smc_input(1, &input, "int", -1);

  int i;

  i = 0;
  for ( ;i < S; )
  {
    {
            __s->smc_set(input, a[i], 32, 32, "int", -1);
    }
    i++;
  }

  gettimeofday(&start,NULL);
  i = 0;
  for ( ;i < rep; )
  {
    //__s->smc_mult(a, a, 32, 32, b, 32, S, "int", -1);
    __s->smc_add(a, a, 32, 32, b, 32, S, "int", -1);
    i++;
  }
  gettimeofday(&end,NULL);
  timerp1 = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
  printf("total time of %d add per run= %ld us\n", S, timerp1/rep);

  gettimeofday(&start,NULL);
  i = 0;
  for ( ;i < rep; )
  {
    __s->smc_mult(a, a, 32, 32, b, 32, S, "int", -1);
    //__s->smc_add(a, a, 32, 32, b, 32, S, "int", -1);
    i++;
  }
  gettimeofday(&end,NULL);
  timerp1 = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
  printf("total time of %d mult per run= %ld us\n", S, timerp1/rep);
  __s->smc_output(1, b, S, "int", -1);

    mpz_clear(input);
  for (int _picco_i = 0; _picco_i < S; _picco_i++)
        mpz_clear(b[_picco_i]);
  free(b);
  for (int _picco_i = 0; _picco_i < S; _picco_i++)
        mpz_clear(a[_picco_i]);
  free(a);
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

 S = atoi(argv[8]);
 rep = atoi(argv[9]);
 
__s = new SMC_Utils(atoi(argv[1]), argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), IO_files, 3, 1, 33, "4294967311", 1);

struct timeval tv1;
struct timeval tv2;  int _xval = 0;

  gettimeofday(&tv1,NULL);
  _xval = (int) __original_main(argc, argv);
  gettimeofday(&tv2, NULL);
  std::cout << "Time: " << __s->time_diff(&tv1,&tv2) << std::endl;
  return (_xval);
}

