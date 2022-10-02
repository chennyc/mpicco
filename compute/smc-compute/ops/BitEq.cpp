#include "BitEq.h"

BitEq::BitEq(NodeNetwork nodeNet, std::map<std::string, std::vector<int>> poly, int NodeID, SecretShare *s, mpz_t coeficients[])
{
    Mul = new Mult(nodeNet, NodeID, s);
    net = nodeNet;
    id = NodeID;
    ss = s;
}

BitEq::BitEq(NodeNetwork nodeNet, std::map<std::string, std::vector<int>> poly, int NodeID, SecretShare *s, mpz_t coeficients[], MaliciousSMC *malicious)
{
    Mul = new Mult(nodeNet, NodeID, s);
    net = nodeNet;
    id = NodeID;
    ss = s;
    ms = malicious;
}

BitEq::~BitEq() {}

void BitEq::doOperation(mpz_t **A, mpz_t **B, mpz_t *Result, int size, int batch_size, int threadID)
{
    // Mul->doOperation(result, A, B, size, threadID);
    mpz_t *mult_Buff1 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size);
    mpz_t *mult_Buff2 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size);
    mpz_t *mult_Buff3 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size);

    mpz_t **s = (mpz_t **)malloc(sizeof(mpz_t *) * batch_size);

    for (int i = 0; i < batch_size * size; i++)
    {
        mpz_init(mult_Buff1[i]);
        mpz_init(mult_Buff2[i]);
        mpz_init(mult_Buff3[i]);
    }

    for (int i = 0; i < batch_size; i++)
    {
        s[i] = (mpz_t *)malloc(sizeof(mpz_t) * size);
        for (int j = 0; j < size; j++)
        {
            mpz_init(s[i][j]);
        }
    }

    mpz_t tmp;
    mpz_init(tmp);

    int buff_p1 = 0;
    int buff_p2 = 0;
    for (int i = 0; i < batch_size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            mpz_set(mult_Buff1[buff_p1], A[i][j]);
            mpz_set(mult_Buff2[buff_p1++], B[i][j]);
        }
    }

    Mul->doOperation(mult_Buff3, mult_Buff1, mult_Buff2, size * batch_size, threadID);

    for (int i = 0; i < batch_size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            ss->modAdd(s[i][j], A[i][j], B[i][j]);
            ss->modSub(s[i][j], s[i][j], mult_Buff3[buff_p2]);
            ss->modSub(s[i][j], s[i][j], mult_Buff3[buff_p2++]);
            ss->modSub(s[i][j], 1, s[i][j]);
        }
    }

    recursiveOp(s, size, batch_size, threadID);

    for (int i = 0; i < batch_size; ++i)
    {
        mpz_set(Result[i], s[i][0]);
    }

    // clear
    for (int i = 0; i < batch_size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            mpz_clear(s[i][j]);
        }
        free(s[i]);
    }
    free(s);

    for (int i = 0; i < size * batch_size; i++)
    {
        mpz_clear(mult_Buff1[i]);
        mpz_clear(mult_Buff2[i]);
        mpz_clear(mult_Buff3[i]);
    }
    free(mult_Buff1);
    free(mult_Buff2);
    free(mult_Buff3);
}

void BitEq::recursiveOp(mpz_t **s, int size, int batch_size, int threadID)
{
    // size represent bit len of a bitwise value, batch_size represnets how many such values.
    int rounds = (int)(log2(size));
    int maxMulSize = batch_size * (size / 2);
    mpz_t *mult_Buff1 = (mpz_t *)malloc(sizeof(mpz_t) * maxMulSize);
    mpz_t *mult_Buff2 = (mpz_t *)malloc(sizeof(mpz_t) * maxMulSize);
    mpz_t *mult_Buff3 = (mpz_t *)malloc(sizeof(mpz_t) * maxMulSize);

    for (int i = 0; i < maxMulSize; i++)
    {
        mpz_init(mult_Buff1[i]);
        mpz_init(mult_Buff2[i]);
        mpz_init(mult_Buff3[i]);
    }

    int buff_p1 = 0;
    int buff_p2 = 0;
    int y = 0;

    int curSize = size;
    while (curSize != 1)
    {
        int curMulSize = curSize / 2;
        buff_p1 = 0;
        buff_p2 = 0;
        for (int i = 0; i < batch_size; ++i)
        {
            for (int j = 0; j < curMulSize; ++j)
            {
                mpz_set(mult_Buff1[buff_p1], s[i][2 * j]);
                mpz_set(mult_Buff2[buff_p1++], s[i][2 * j + 1]);
            }
        }

        Mul->doOperation(mult_Buff3, mult_Buff1, mult_Buff2, curMulSize * batch_size, threadID);

        for (int i = 0; i < batch_size; ++i)
        {
            for (int j = 0; j < curMulSize; ++j)
            {
                mpz_set(s[i][j], mult_Buff3[buff_p2++]);
            }
        }

        if (curSize % 2 == 1)
        {
            for (int i = 0; i < batch_size; ++i)
            {
                mpz_set(s[i][curMulSize], s[i][curSize - 1]);
            }
        }
        curSize = curSize / 2 + curSize % 1;
    }

    // clear
    for (int i = 0; i < maxMulSize; i++)
    {
        mpz_clear(mult_Buff1[i]);
        mpz_clear(mult_Buff2[i]);
        mpz_clear(mult_Buff3[i]);
    }
    free(mult_Buff1);
    free(mult_Buff2);
    free(mult_Buff3);
}

void BitEq::doOperation_mal(mpz_t ***A, mpz_t ***B, mpz_t **Result, int size, int batch_size, int threadID)
{
    // Mul->doOperation(result, A, B, size, threadID);
    mpz_t *mult_Buff1 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size * 2);
    mpz_t *mult_Buff2 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size * 2);
    mpz_t *mult_Buff3 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size * 2);

    for (int i = 0; i < batch_size * size * 2; i++)
    {
        mpz_init(mult_Buff1[i]);
        mpz_init(mult_Buff2[i]);
        mpz_init(mult_Buff3[i]);
    }

    mpz_t ***s = (mpz_t ***)malloc(sizeof(mpz_t **) * 2);
    for (int mi = 0; mi < 2; ++mi)
    {
        s[mi] = (mpz_t **)malloc(sizeof(mpz_t *) * batch_size);
        for (int i = 0; i < batch_size; i++)
        {
            s[mi][i] = (mpz_t *)malloc(sizeof(mpz_t) * size);
            for (int j = 0; j < size; j++)
            {
                mpz_init(s[mi][i][j]);
            }
        }
    }
    mpz_t tmp;
    mpz_init(tmp);

    int buff_p1 = 0;
    int buff_p2 = 0;
    for (int i = 0; i < batch_size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            mpz_set(mult_Buff1[buff_p1], A[0][i][j]);
            mpz_set(mult_Buff2[buff_p1++], B[0][i][j]);
        }
    }

    for (int i = 0; i < batch_size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            mpz_set(mult_Buff1[buff_p1], A[0][i][j]);
            mpz_set(mult_Buff2[buff_p1++], B[1][i][j]);
        }
    }

    Mul->doOperation(mult_Buff3, mult_Buff1, mult_Buff2, size * batch_size * 2, threadID);
    ms->pushBuffer(mult_Buff3, &mult_Buff3[size * batch_size], size * batch_size);

    for (int i = 0; i < batch_size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            ss->modAdd(s[0][i][j], A[0][i][j], B[0][i][j]);
            ss->modSub(s[0][i][j], s[0][i][j], mult_Buff3[buff_p2]);
            ss->modSub(s[0][i][j], s[0][i][j], mult_Buff3[buff_p2]);
            ss->modSub(s[0][i][j], 1, s[0][i][j]);

            ss->modAdd(s[1][i][j], A[1][i][j], B[1][i][j]);
            ss->modSub(s[1][i][j], s[1][i][j], mult_Buff3[buff_p2 + size * batch_size]);
            ss->modSub(s[1][i][j], s[1][i][j], mult_Buff3[buff_p2 + size * batch_size]);
            ss->modSub(s[1][i][j], ms->r, s[1][i][j]);
        }
    }

    recursiveOp_mal(s, size, batch_size, threadID);

    for (int i = 0; i < batch_size; ++i)
    {
        mpz_set(Result[0][i], s[0][i][0]);
        mpz_set(Result[1][i], s[1][i][0]);
    }

    // clear
    for (int mi = 0; mi < 2; ++mi)
    {
        for (int i = 0; i < batch_size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                mpz_clear(s[mi][i][j]);
            }
            free(s[mi][i]);
        }
        free(s[mi]);
    }
    free(s);

    for (int i = 0; i < size * batch_size * 2; i++)
    {
        mpz_clear(mult_Buff1[i]);
        mpz_clear(mult_Buff2[i]);
        mpz_clear(mult_Buff3[i]);
    }
    free(mult_Buff1);
    free(mult_Buff2);
    free(mult_Buff3);
}

void BitEq::recursiveOp_mal(mpz_t ***s, int size, int batch_size, int threadID)
{
    // size represent bit len of a bitwise value, batch_size represnets how many such values.
    int rounds = (int)(log2(size));
    int maxMulSize = batch_size * (size / 2);
    mpz_t *mult_Buff1 = (mpz_t *)malloc(sizeof(mpz_t) * maxMulSize * 2);
    mpz_t *mult_Buff2 = (mpz_t *)malloc(sizeof(mpz_t) * maxMulSize * 2);
    mpz_t *mult_Buff3 = (mpz_t *)malloc(sizeof(mpz_t) * maxMulSize * 2);

    for (int i = 0; i < maxMulSize * 2; i++)
    {
        mpz_init(mult_Buff1[i]);
        mpz_init(mult_Buff2[i]);
        mpz_init(mult_Buff3[i]);
    }

    int buff_p1 = 0;
    int buff_p2 = 0;
    int y = 0;

    int curSize = size;
    while (curSize != 1)
    {
        int curMulSize = curSize / 2;
        buff_p1 = 0;
        buff_p2 = 0;
        for (int i = 0; i < batch_size; ++i)
        {
            for (int j = 0; j < curMulSize; ++j)
            {
                mpz_set(mult_Buff1[buff_p1], s[0][i][2 * j]);
                mpz_set(mult_Buff2[buff_p1++], s[0][i][2 * j + 1]);
            }
        }

        for (int i = 0; i < batch_size; ++i)
        {
            for (int j = 0; j < curMulSize; ++j)
            {
                mpz_set(mult_Buff1[buff_p1], s[0][i][2 * j]);
                mpz_set(mult_Buff2[buff_p1++], s[1][i][2 * j + 1]);
            }
        }

        Mul->doOperation(mult_Buff3, mult_Buff1, mult_Buff2, curMulSize * batch_size * 2, threadID);
        ms->pushBuffer(mult_Buff3, &mult_Buff3[curMulSize * batch_size], curMulSize * batch_size);

        for (int i = 0; i < batch_size; ++i)
        {
            for (int j = 0; j < curMulSize; ++j)
            {
                mpz_set(s[0][i][j], mult_Buff3[buff_p2]);
                mpz_set(s[1][i][j], mult_Buff3[buff_p2 + curMulSize * batch_size]);
                buff_p2++;
            }
        }

        if (curSize % 2 == 1)
        {
            for (int i = 0; i < batch_size; ++i)
            {
                mpz_set(s[0][i][curMulSize], s[0][i][curSize - 1]);
                mpz_set(s[1][i][curMulSize], s[1][i][curSize - 1]);
            }
        }
        curSize = curSize / 2 + curSize % 1;
    }

    // clear
    for (int i = 0; i < maxMulSize; i++)
    {
        mpz_clear(mult_Buff1[i]);
        mpz_clear(mult_Buff2[i]);
        mpz_clear(mult_Buff3[i]);
    }
    free(mult_Buff1);
    free(mult_Buff2);
    free(mult_Buff3);
}

int BitEq::open(mpz_t var, int threadID)
{
    int peers = ss->getPeers();
    mpz_t *data = (mpz_t *)malloc(sizeof(mpz_t) * 1);
    mpz_t *results = (mpz_t *)malloc(sizeof(mpz_t) * 1);
    mpz_t **buffer = (mpz_t **)malloc(sizeof(mpz_t *) * peers);
    for (int i = 0; i < peers; ++i)
    {
        buffer[i] = (mpz_t *)malloc(sizeof(mpz_t));
        mpz_init(buffer[i][0]);
    }
    mpz_init(data[0]);
    mpz_init(results[0]);
    mpz_set(data[0], var);
    net.broadcastToPeers(data, 1, buffer, threadID);
    ss->reconstructSecret(results, buffer, 1, false);
    mpz_t tmp, field;
    mpz_init(tmp);
    mpz_init(field);
    ss->getFieldSize(field);
    mpz_mul_ui(tmp, results[0], 2);
    if (mpz_cmp(tmp, field) > 0)
        mpz_sub(results[0], results[0], field);
    // gmp_printf("%Zd ", results[0]);
    int result = mpz_get_si(results[0]);
    // free the memory
    mpz_clear(data[0]);
    mpz_clear(results[0]);
    free(data);
    free(results);
    for (int i = 0; i < peers; i++)
    {
        mpz_clear(buffer[i][0]);
        free(buffer[i]);
    }
    free(buffer);

    return result;
}

void BitEq::reveal(mpz_t *toBeOpen, int size, int threadID)
{
    int i = 0;
    int res = 0;
    for (i = 0; i < size; ++i)
    {
        res = open(toBeOpen[i], threadID);
        printf(" %d ", res);
    }
    printf("\n");
}