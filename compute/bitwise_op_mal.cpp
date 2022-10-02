/* File generated from [mult-add_Batch.c] by PICCO Tue Nov 10 19:43:49 2020
 */

#include <limits.h>

#include <float.h>

// extern void *memcpy(void*,const void*,unsigned int);

//# 1 "ort.onoff.defs"

extern "C" int ort_initialize(int *, char ***);
extern "C" void ort_finalize(int);

/* File generated from [mult-add_Batch.c] by PICCO Tue Nov 10 19:43:49 2020
 */

#include "smc-compute/SMC_Utils.h"

#include <gmp.h>

#include <omp.h>

SMC_Utils *__s;

int S = 32;
int bS = 10;
int rep = 1;
mpz_t r;

int __original_main(int _argc_ignored, char **_argv_ignored)
{
    struct timeval start;
    struct timeval end;
    unsigned long timerp1;
    struct timeval start1;
    struct timeval end1;
    unsigned long timer1 = 0;
    mpz_t _picco_tmp1;
    mpz_init(_picco_tmp1);

    mpz_t *_picco_ftmp1 = (mpz_t *)malloc(sizeof(mpz_t) * 4);
    for (int _picco_j = 0; _picco_j < 4; _picco_j++)
        mpz_init(_picco_ftmp1[_picco_j]);
    void *_picco_temp_;

    mpz_t ***a;
    mpz_t ***b;
    mpz_t ***c;
    a = (mpz_t ***)malloc(sizeof(mpz_t **) * 2);
    b = (mpz_t ***)malloc(sizeof(mpz_t **) * 2);
    c = (mpz_t ***)malloc(sizeof(mpz_t **) * 2);
    for (int mi = 0; mi < 2; ++mi)
    {
        a[mi] = (mpz_t **)malloc(sizeof(mpz_t *) * (bS));
        b[mi] = (mpz_t **)malloc(sizeof(mpz_t *) * (bS));
        c[mi] = (mpz_t **)malloc(sizeof(mpz_t *) * (bS));
        for (int _picco_i = 0; _picco_i < bS; _picco_i++)
        {
            a[mi][_picco_i] = (mpz_t *)malloc(sizeof(mpz_t) * (S));
            b[mi][_picco_i] = (mpz_t *)malloc(sizeof(mpz_t) * (S));
            c[mi][_picco_i] = (mpz_t *)malloc(sizeof(mpz_t) * (S+1));
            for (int j = 0; j < S; ++j)
            {
                mpz_init(a[mi][_picco_i][j]);
                mpz_init(b[mi][_picco_i][j]);
                mpz_init(c[mi][_picco_i][j]);
            }
            mpz_init(c[mi][_picco_i][S]);
        }
    }

    mpz_t **res;
    res = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
    for (int mi = 0; mi < 2; ++mi)
    {
        res[mi] = (mpz_t *)malloc(sizeof(mpz_t) * (bS));
        for (int _picco_i = 0; _picco_i < bS; _picco_i++)
            mpz_init(res[mi][_picco_i]);
    }

    mpz_t **input;
    input = (mpz_t **)malloc(sizeof(mpz_t *) * 2);
    for(int mi = 0; mi < 2; ++mi) {
        input[mi] = (mpz_t *)malloc(sizeof(mpz_t) * 1);
        mpz_init(input[mi][0]);
    }
    mpz_set_ui(input[0][0], 1);
    __s->smc_input_Onlymul(input, 1, 1, 128, "int", -1);

    //__s->smc_input(1, &input, "int", -1);

    int i;

    i = 0;
    for (i = 0; i < bS; ++i)
    {
        for (int j = 0; j < S; ++j)
        {
            mpz_set(a[0][i][j], input[0][0]);
            mpz_set(b[0][i][j], input[0][0]);

            mpz_set(a[1][i][j], input[1][0]);
            mpz_set(b[1][i][j], input[1][0]);
        }
    }

    printf("start add\n");
    gettimeofday(&start, NULL);
    i = 0;
    for (; i < rep;)
    {
        __s->smc_bitadd_mal(a, b, c, S, bS, -1);
        gettimeofday(&start1, NULL);
        __s->smc_verify();
        gettimeofday(&end1, NULL);
        timer1 += 1000000 * (end1.tv_sec - start1.tv_sec) + end1.tv_usec - start1.tv_usec;
        i++;
    }
    gettimeofday(&end, NULL);
    timerp1 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("total time of %d add per run= %ld us\n", S, timerp1 / rep);
    printf("verification time per run = %ld us\n", timer1 / rep);

    printf("start lt\n");
    gettimeofday(&start, NULL);
    i = 0;
    timer1 = 0;
    for (; i < rep;)
    {
        __s->smc_bitcomp_mal(a, b, res, S, bS, -1);
        gettimeofday(&start1, NULL);
        __s->smc_verify();
        gettimeofday(&end1, NULL);
        timer1 += 1000000 * (end1.tv_sec - start1.tv_sec) + end1.tv_usec - start1.tv_usec;
        i++;
    }
    gettimeofday(&end, NULL);
    timerp1 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("total time of %d lt per run = %ld us\n", S, timerp1 / rep);
    printf("verification time per run = %ld us\n", timer1 / rep);

    printf("start eq\n");
    gettimeofday(&start, NULL);
    i = 0;
    timer1 = 0;
    for (; i < rep;)
    {
        __s->smc_biteq_mal(a, b, res, S, bS, -1);
        gettimeofday(&start1, NULL);
        __s->smc_verify();
        gettimeofday(&end1, NULL);
        timer1 += 1000000 * (end1.tv_sec - start1.tv_sec) + end1.tv_usec - start1.tv_usec;
        i++;
    }
    gettimeofday(&end, NULL);
    timerp1 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("total time of %d eq per run = %ld us\n", S, timerp1 / rep);
    printf("verification time per run = %ld us\n", timer1 / rep);
    /*
      int *openResult = new int[9];

      printf("a is \n");
      for (int i = 0; i < bS; ++i)
      {
        __s->smc_open_batch(a[i], openResult, S, -1);
        printf("\n a[%d] is :\n", i);
        for (int j = 0; j < S; ++j)
        {
          printf("%d ", openResult[j]);
        }
      }

      printf("b is \n");
      for (int i = 0; i < bS; ++i)
      {
        __s->smc_open_batch(b[i], openResult, S, -1);
        printf("\n b[%d] is :\n", i);
        for (int j = 0; j < S; ++j)
        {
          printf("%d ", openResult[j]);
        }
      }

      printf("c is \n");
      for (int i = 0; i < bS; ++i)
      {
        __s->smc_open_batch(c[i], openResult, S + 1, -1);
        printf("\n c[%d] is :\n", i);
        for (int j = 0; j < S + 1; ++j)
        {
          printf("%d ", openResult[j]);
        }
      }
      int *resopenResult = new int[bS];
      __s->smc_open_batch(res, resopenResult, bS, -1);
      printf("\n res is :\n");
      for (int j = 0; j < bS; ++j)
      {
        printf("%d ", resopenResult[j]);
      }
    */
    //__s->smc_output(1, b, S, "int", -1);


    for (int mi = 0; mi < 2; ++mi)
    {
        for (int i = 0; i < bS; i++)
        {
            for (int j = 0; j < S; j++)
            {
                mpz_clear(a[mi][i][j]);
                mpz_clear(b[mi][i][j]);
                mpz_clear(c[mi][i][j]);
            }
            mpz_clear(c[mi][i][S]);
            mpz_clear(res[mi][i]);
            free(a[mi][i]);
            free(b[mi][i]);
            free(c[mi][i]);
        }
        free(a[mi]);
        free(b[mi]);
        free(c[mi]);
        free(res[mi]);
        mpz_clear(input[mi][0]);
        free(input[mi]);
    }
    free(a);
    free(b);
    free(c);
    free(res);
    free(input);

    mpz_clear(_picco_tmp1);
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
        fprintf(stderr, "Usage: <id> <runtime-config> <privatekey-filename> <number-of-input-parties> <number-of-output-parties> <input-share> <output> <bS> <rep>\n");
        exit(1);
    }

    std::string IO_files[atoi(argv[4]) + atoi(argv[5])];
    for (int i = 0; i < 2; i++)
        IO_files[i] = argv[6 + i];

    bS = atoi(argv[8]);
    rep = atoi(argv[9]);

    __s = new SMC_Utils(atoi(argv[1]), argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), IO_files, 3, 1, 129, "340282366920938463463374607431768211507", 1);

    struct timeval tv1;
    struct timeval tv2;
    int _xval = 0;

    gettimeofday(&tv1, NULL);
      mpz_init(r);
  __s->smc_init_mal(); // generate r and initialize buffer for verification triples
    _xval = (int)__original_main(argc, argv);
    __s->smc_clean_mal();
    gettimeofday(&tv2, NULL);
    std::cout << "Time: " << __s->time_diff(&tv1, &tv2) << std::endl;
    return (_xval);
}
