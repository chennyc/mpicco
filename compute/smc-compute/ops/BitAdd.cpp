#include "BitAdd.h"

BitAdd::BitAdd(NodeNetwork nodeNet, std::map<std::string, std::vector<int>> poly, int NodeID, SecretShare *s, mpz_t coeficients[])
{
  Mul = new Mult(nodeNet, NodeID, s);
  net = nodeNet;
  id = NodeID;
  ss = s;
}

BitAdd::BitAdd(NodeNetwork nodeNet, std::map<std::string, std::vector<int>> poly, int NodeID, SecretShare *s, mpz_t coeficients[], MaliciousSMC *malicious)
{
  Mul = new Mult(nodeNet, NodeID, s);
  ms = malicious;
  net = nodeNet;
  id = NodeID;
  ss = s;
}

BitAdd::~BitAdd() {}

void BitAdd::doOperation(mpz_t **A, mpz_t **B, mpz_t **S, int size, int batch_size, int threadID)
{
  // Mul->doOperation(result, A, B, size, threadID);
  mpz_t *mult_Buff1 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size);
  mpz_t *mult_Buff2 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size);
  mpz_t *mult_Buff3 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size);

  mpz_t **p = (mpz_t **)malloc(sizeof(mpz_t *) * batch_size);
  mpz_t **g = (mpz_t **)malloc(sizeof(mpz_t *) * batch_size);

  for (int i = 0; i < batch_size * size; i++)
  {
    mpz_init(mult_Buff1[i]);
    mpz_init(mult_Buff2[i]);
    mpz_init(mult_Buff3[i]);
  }

  for (int i = 0; i < batch_size; i++)
  {
    p[i] = (mpz_t *)malloc(sizeof(mpz_t) * size);
    g[i] = (mpz_t *)malloc(sizeof(mpz_t) * size);
    for (int j = 0; j < size; j++)
    {
      mpz_init(p[i][j]);
      mpz_init(g[i][j]);
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
      mpz_set(p[i][j], mult_Buff3[buff_p2++]);
      ss->modAdd(g[i][j], A[i][j], B[i][j]);
      ss->modMul(tmp, p[i][j], 2);
      ss->modSub(g[i][j], g[i][j], tmp);
    }
  }

  prefixCarry(p, g, size, batch_size, threadID);

  for (int i = 0; i < batch_size; ++i)
  {
    mpz_set(g[i][0], mult_Buff3[i * size]);
    ss->modAdd(S[i][0], A[i][0], B[i][0]);
    ss->modMul(tmp, g[i][0], 2);
    ss->modSub(S[i][0], S[i][0], tmp);
  }

  for (int i = 0; i < batch_size; ++i)
  {
    for (int j = 1; j < size; ++j)
    {
      ss->modAdd(S[i][j], A[i][j], B[i][j]);
      ss->modMul(tmp, g[i][j], 2);
      ss->modSub(S[i][j], S[i][j], tmp);
      ss->modAdd(S[i][j], S[i][j], g[i][j - 1]);
    }
    // if there is size + 1
    mpz_set(S[i][size], g[i][size - 1]);
  }

  // clear
  for (int i = 0; i < batch_size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      mpz_clear(p[i][j]);
      mpz_clear(g[i][j]);
    }
    free(p[i]);
    free(g[i]);
  }
  free(p);
  free(g);

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

void BitAdd::prefixCarry(mpz_t **p, mpz_t **g, int size, int batch_size, int threadID)
{
  // size represent bit len of a bitwise value, batch_size represnets how many such values.
  // Mul->doOperation(result, A, B, size, threadID);
  int rounds = (int)ceil(log2(size));
  mpz_t *mult_Buff1 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size);
  mpz_t *mult_Buff2 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size);
  mpz_t *mult_Buff3 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size);

  for (int i = 0; i < batch_size * size; i++)
  {
    mpz_init(mult_Buff1[i]);
    mpz_init(mult_Buff2[i]);
    mpz_init(mult_Buff3[i]);
  }

  int buff_p1 = 0;
  int buff_p2 = 0;
  int y = 0;
  mpz_t tmp;
  mpz_init(tmp);

  for (int i = 0; i < rounds; ++i)
  {
    y = pow(2, i) - 1;
    buff_p1 = 0;
    for (int j = 0; j < size / (int)pow(2, i + 1); ++j)
    {
      for (int z = 1; z <= pow(2, i); ++z)
      {
        if (y >= size || y + z >= size)
        {
          continue;
        }
        for (int k = 0; k < batch_size; ++k)
        {
          mpz_set(mult_Buff1[buff_p1], p[k][y + z]);
          mpz_set(mult_Buff2[buff_p1++], p[k][y]);

          mpz_set(mult_Buff1[buff_p1], p[k][y]);
          mpz_set(mult_Buff2[buff_p1++], g[k][y + z]);
        }
      }
      y += pow(2, i + 1);
    }
    Mul->doOperation(mult_Buff3, mult_Buff1, mult_Buff2, size * batch_size, threadID);
    y = pow(2, i) - 1;
    buff_p2 = 0;
    for (int j = 0; j < size / pow(2, i + 1); ++j)
    {
      for (int z = 1; z <= pow(2, i); ++z)
      {
        if (y >= size || y + z >= size)
        {
          continue;
        }
        // a_y+z = a_y op a_y+z
        for (int k = 0; k < batch_size; ++k)
        {
          mpz_set(p[k][y + z], mult_Buff3[buff_p2++]);
          ss->modAdd(g[k][y + z], g[k][y], mult_Buff3[buff_p2++]);
        }
      }
      y += pow(2, i + 1);
    }
  }

  // clear
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

void BitAdd::doOperation_mal(mpz_t ***A, mpz_t ***B, mpz_t ***S, int size, int batch_size, int threadID)
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

  mpz_t ***p = (mpz_t ***)malloc(sizeof(mpz_t **) * 2);
  mpz_t ***g = (mpz_t ***)malloc(sizeof(mpz_t **) * 2);

  for (int mi = 0; mi < 2; mi++)
  {
    p[mi] = (mpz_t **)malloc(sizeof(mpz_t *) * batch_size);
    g[mi] = (mpz_t **)malloc(sizeof(mpz_t *) * batch_size);
    for (int i = 0; i < batch_size; i++)
    {
      p[mi][i] = (mpz_t *)malloc(sizeof(mpz_t) * size);
      g[mi][i] = (mpz_t *)malloc(sizeof(mpz_t) * size);
      for (int j = 0; j < size; j++)
      {
        mpz_init(p[mi][i][j]);
        mpz_init(g[mi][i][j]);
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

  Mul->doOperation(mult_Buff3, mult_Buff1, mult_Buff2, 2 * size * batch_size, threadID);
  ms->pushBuffer(mult_Buff3, &mult_Buff3[size * batch_size], size * batch_size);
  for (int i = 0; i < batch_size; ++i)
  {
    for (int j = 0; j < size; ++j)
    {
      mpz_set(g[0][i][j], mult_Buff3[buff_p2]);
      ss->modAdd(p[0][i][j], A[0][i][j], B[0][i][j]);
      ss->modMul(tmp, g[0][i][j], 2);
      ss->modSub(p[0][i][j], p[0][i][j], tmp);

      mpz_set(g[1][i][j], mult_Buff3[buff_p2 + size * batch_size]);
      ss->modAdd(p[1][i][j], A[1][i][j], B[1][i][j]);
      ss->modMul(tmp, g[1][i][j], 2);
      ss->modSub(p[1][i][j], p[1][i][j], tmp);
      buff_p2++;
    }
  }

  prefixCarry_mal(p, g, size, batch_size, threadID);

  for (int i = 0; i < batch_size; ++i)
  {
    mpz_set(g[0][i][0], mult_Buff3[i * size]);
    ss->modAdd(S[0][i][0], A[0][i][0], B[0][i][0]);
    ss->modMul(tmp, g[0][i][0], 2);
    ss->modSub(S[0][i][0], S[0][i][0], tmp);

    mpz_set(g[1][i][0], mult_Buff3[i * size + size * batch_size]);
    ss->modAdd(S[1][i][0], A[1][i][0], B[1][i][0]);
    ss->modMul(tmp, g[1][i][0], 2);
    ss->modSub(S[1][i][0], S[1][i][0], tmp);
  }

  for (int i = 0; i < batch_size; ++i)
  {
    for (int j = 1; j < size; ++j)
    {
      ss->modAdd(S[0][i][j], A[0][i][j], B[0][i][j]);
      ss->modMul(tmp, g[0][i][j], 2);
      ss->modSub(S[0][i][j], S[0][i][j], tmp);
      ss->modAdd(S[0][i][j], S[0][i][j], g[0][i][j - 1]);

      ss->modAdd(S[1][i][j], A[1][i][j], B[1][i][j]);
      ss->modMul(tmp, g[1][i][j], 2);
      ss->modSub(S[1][i][j], S[1][i][j], tmp);
      ss->modAdd(S[1][i][j], S[1][i][j], g[1][i][j - 1]);
    }
    // if there is size + 1
    // mpz_set(S[i][size], g[i][size - 1]);
  }

  // clear
  for (int mi = 0; mi < 2; ++mi)
  {
    for (int i = 0; i < batch_size; i++)
    {
      for (int j = 0; j < size; j++)
      {
        mpz_clear(p[mi][i][j]);
        mpz_clear(g[mi][i][j]);
      }
      free(p[mi][i]);
      free(g[mi][i]);
    }
    free(p[mi]);
    free(g[mi]);
  }
  free(p);
  free(g);

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

void BitAdd::prefixCarry_mal(mpz_t ***p, mpz_t ***g, int size, int batch_size, int threadID)
{
  // size represent bit len of a bitwise value, batch_size represnets how many such values.
  // Mul->doOperation(result, A, B, size, threadID);
  int rounds = (int)ceil(log2(size));
  mpz_t *mult_Buff1 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size * 2);
  mpz_t *mult_Buff2 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size * 2);
  mpz_t *mult_Buff3 = (mpz_t *)malloc(sizeof(mpz_t) * size * batch_size * 2);

  for (int i = 0; i < batch_size * size * 2; i++)
  {
    mpz_init(mult_Buff1[i]);
    mpz_init(mult_Buff2[i]);
    mpz_init(mult_Buff3[i]);
  }

  int buff_p1 = 0;
  int buff_p2 = 0;
  int y = 0;
  mpz_t tmp;
  mpz_init(tmp);

  for (int i = 0; i < rounds; ++i)
  {
    y = pow(2, i) - 1;
    buff_p1 = 0;
    for (int j = 0; j < size / (int)pow(2, i + 1); ++j)
    {
      for (int z = 1; z <= pow(2, i); ++z)
      {
        if (y >= size || y + z >= size)
        {
          continue;
        }
        for (int k = 0; k < batch_size; ++k)
        {
          mpz_set(mult_Buff1[buff_p1], p[0][k][y + z]);
          mpz_set(mult_Buff2[buff_p1++], p[0][k][y]);

          mpz_set(mult_Buff1[buff_p1], p[0][k][y]);
          mpz_set(mult_Buff2[buff_p1++], g[0][k][y + z]);
        }

        for (int k = 0; k < batch_size; ++k)
        {
          mpz_set(mult_Buff1[buff_p1], p[0][k][y + z]);
          mpz_set(mult_Buff2[buff_p1++], p[1][k][y]);

          mpz_set(mult_Buff1[buff_p1], p[0][k][y]);
          mpz_set(mult_Buff2[buff_p1++], g[1][k][y + z]);
        }
      }
      y += pow(2, i + 1);
    }
    Mul->doOperation(mult_Buff3, mult_Buff1, mult_Buff2, size * batch_size * 2, threadID);
    ms->pushBuffer(mult_Buff3, &mult_Buff3[size * batch_size], size * batch_size);

    y = pow(2, i) - 1;
    buff_p2 = 0;
    for (int j = 0; j < size / pow(2, i + 1); ++j)
    {
      for (int z = 1; z <= pow(2, i); ++z)
      {
        if (y >= size || y + z >= size)
        {
          continue;
        }
        // a_y+z = a_y op a_y+z
        for (int k = 0; k < batch_size; ++k)
        {
          mpz_set(p[0][k][y + z], mult_Buff3[buff_p2]);
          mpz_set(p[1][k][y + z], mult_Buff3[size * batch_size + buff_p2]);
          buff_p2++;
          ss->modAdd(g[0][k][y + z], g[0][k][y], mult_Buff3[buff_p2]);
          ss->modAdd(g[1][k][y + z], g[1][k][y], mult_Buff3[size * batch_size + buff_p2]);
          buff_p2++;
        }
      }
      y += pow(2, i + 1);
    }
  }

  // clear
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

int BitAdd::open(mpz_t var, int threadID)
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

void BitAdd::reveal(mpz_t *toBeOpen, int size, int threadID)
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