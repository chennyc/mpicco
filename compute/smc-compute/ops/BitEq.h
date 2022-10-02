#ifndef BITEQ_H_
#define BITEQ_H_

#include "Mult.h"
#include "Operation.h"

class BitEq : public Operation{
public:
        BitEq(NodeNetwork nodeNet, std::map<std::string, std::vector<int> > poly, int nodeID, SecretShare *s, mpz_t coeficients[]);
        BitEq(NodeNetwork nodeNet, std::map<std::string, std::vector<int> > poly, int nodeID, SecretShare *s, mpz_t coeficients[], MaliciousSMC *malicious);
        virtual ~BitEq();
        void doOperation(mpz_t** A, mpz_t** B, mpz_t* S, int size, int batch_size, int threadID);
        void recursiveOp(mpz_t** p, int size, int batch_size, int threadID);
        void doOperation_mal(mpz_t*** A, mpz_t*** B, mpz_t** S, int size, int batch_size, int threadID);
        void recursiveOp_mal(mpz_t*** p, int size, int batch_size, int threadID);
        int open(mpz_t var, int threadID);
        void reveal(mpz_t* toBeOpen, int size, int threadID);
private:
        Mult *Mul;
};

#endif /* BITEQ_H_ */
