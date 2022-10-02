#ifndef MALICIOUSSMC_H_
#define MALICIOUSSMC_H_

#include "stdint.h"
#include <vector>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <gmp.h>
#include "NodeNetwork.h"
#include "SecretShare.h"
#include "ops/Mult.h"

class MaliciousSMC
{
public:
    MaliciousSMC(NodeNetwork nodeNet, SecretShare* s, int p, int t);
    MaliciousSMC();
	virtual ~MaliciousSMC();

    // malicious functions
    void init_mal();
    void clean_mal();
    // void input_mal(int id, mpz_t* var, int size, int varlen, std::stringtype, int threadID);
    void verify();
    void pushBuffer(mpz_t *result, mpz_t *resultR, int size);
    bool checkBuffer(int size);
    void computeLagrangeWeight_mal();
    void reconstructSecretCheck(mpz_t *result, mpz_t **y, int size, bool isMultiply);

    NodeNetwork net;
    SecretShare *ss;
    int peers;
    int threshhold;
    int pid;
    // malicious variabls
    int getcurRow();
    int open(mpz_t var, int threadID);
    void malBufferReset();

    mpz_t r;
    mpz_t **buffer_mal;
    int curPoint_mal; // the last avaiavle element in a row
    int curRow_mal;   // current last used row
    int curCap_mal;   // row intitalized
    int capacity_mal; // total capacity limitation
    int maxRow_mal;   // max number of rows
    int rowCap_mal;   // size of each row
    int curCapacity_mal;
    unsigned long VerificationTimer;

    mpz_t** lagrangeWeight_mal; 

    Mult *Mul;
};

#endif /* MALICIOUSSMC_H_ */
