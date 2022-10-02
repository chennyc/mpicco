/* File generated from [./rss_test/mult.c] by PICCO Sat Nov 21 21:51:39 2020
 */

#include <limits.h>

#include <float.h>

//extern void *memcpy(void*,const void*,unsigned int);

//# 1 "ort.onoff.defs"

extern  "C" int   ort_initialize(int*, char***);
 extern "C" void  ort_finalize(int);


/* File generated from [./rss_test/mult.c] by PICCO Sat Nov 21 21:51:39 2020
 */

#include "smc-compute/SMC_Utils.h"

#include <gmp.h>

#include <omp.h>


SMC_Utils *__s;

int size = 0;
int rep = 0;

int  __original_main(int _argc_ignored, char **_argv_ignored)
{
  	struct timeval start;
	struct timeval end;
	unsigned long timer;

  mpz_t _picco_tmp1;
  mpz_init(_picco_tmp1);

  mpz_t* _picco_ftmp1 = (mpz_t*)malloc(sizeof(mpz_t) * 4);
  for(int _picco_j = 0; _picco_j < 4; _picco_j++)
    mpz_init(_picco_ftmp1[_picco_j]);
  void* _picco_temp_;
 int i, S;

  S = size;
	printf("mult test with batch size %d \n", S);

  mpz_t* A; 
  A = (mpz_t*)malloc(sizeof(mpz_t) * (S));
  for (int _picco_i = 0; _picco_i < S; _picco_i++)
        mpz_init(A[_picco_i]);
  mpz_t* B; 
  B = (mpz_t*)malloc(sizeof(mpz_t) * (S));
  for (int _picco_i = 0; _picco_i < S; _picco_i++)
        mpz_init(B[_picco_i]);
  mpz_t* C; 
  C = (mpz_t*)malloc(sizeof(mpz_t) * (S));
  for (int _picco_i = 0; _picco_i < S; _picco_i++)
        mpz_init(C[_picco_i]);

  mpz_t a;
  mpz_init(a);
  mpz_t b;
  mpz_init(b);

  __s->smc_input(1, &a, "int", -1);

  __s->smc_input(1, &b, "int", -1);

  mpz_set_ui(a, 2);
  mpz_set_ui(b, 3);

  i = 0;
  for ( ;i < S; )
  {
    {
            __s->smc_set(a, A[i], 30, 30, "int", -1);

            __s->smc_set(b, B[i], 30, 30, "int", -1);
    }
    i++;
  }

	gettimeofday(&start,NULL); //start timer here


  i = 0;
  for ( ;i < rep; )
  {
    if(i % 100 == 0)
      printf("%d th run start \n", i);
      
    __s->smc_mult(A, B, 30, 30, C, 30, S, "int", -1);
    i++;
  }

  printf("res is %d \n", __s->smc_open(C[0], -1));

	gettimeofday(&end,NULL);//stop timer here
	timer = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
	printf("runtime for open with data size %d = %ld us\n", size, timer);

  __s->smc_output(1, &C[0], "int", -1);

    mpz_clear(b);
  mpz_clear(a);
  for (int _picco_i = 0; _picco_i < S; _picco_i++)
        mpz_clear(C[_picco_i]);
  free(C);
  for (int _picco_i = 0; _picco_i < S; _picco_i++)
        mpz_clear(B[_picco_i]);
  free(B);
  for (int _picco_i = 0; _picco_i < S; _picco_i++)
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

 if(argc < 11){
fprintf(stderr,"Incorrect input parameters\n");
fprintf(stderr,"Usage: <id> <runtime-config> <privatekey-filename> <number-of-input-parties> <number-of-output-parties> <input-share> <output> <size> <rep> <number of party>\n");
exit(1);
}

 std::string IO_files[atoi(argv[4]) + atoi(argv[5])];
for(int i = 0; i < 2; i++)
   IO_files[i] = argv[6+i];

size = atoi(argv[8]);
rep = atoi(argv[9]);

__s = new SMC_Utils(atoi(argv[1]), argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), IO_files, atoi(argv[10]), atoi(argv[10]) / 2, 31, "1073741827", 1);
 printf("smc setup done \n");
struct timeval tv1;
struct timeval tv2;  int _xval = 0;

  gettimeofday(&tv1,NULL);
  _xval = (int) __original_main(argc, argv);
  gettimeofday(&tv2, NULL);
  std::cout << "Time: " << __s->time_diff(&tv1,&tv2) << std::endl;
  //__s->smc_communication_summary();
  return (_xval);
}

