#include "MaliciousSMC.h"
#include <sys/time.h> 

MaliciousSMC::MaliciousSMC(NodeNetwork nodeNet, SecretShare* s, int p, int t)
{
    net = nodeNet;
	ss = s;
    VerificationTimer = 0;
	peers = p;
	threshhold = t;
	pid = net.getID();

	Mul = new Mult(nodeNet, pid, s);

	init_mal();
}

MaliciousSMC::MaliciousSMC() {}

MaliciousSMC::~MaliciousSMC() {}

// malicious functions
int MaliciousSMC::getcurRow()
{
	return curRow_mal;
}

void MaliciousSMC::init_mal()
{
	printf("Hello Malicious Mode\n");
	// This function is used to initialize all variables and buffers for malicious version.
	// r will be assigned a random value later in SMC
	// Initialize buffer
	// The verification buffer is organized as a 2-D arrays, i.e., buffer_mal[maxRow_mal][rowCap_mal]. 
	// In initalization, we only alloc memory for the first curCap_mal rows. At this point, 
	// our buffer could contain curCap_mal * rowCap_mal elements. 
	// As elements being pushed into buffer, the initalized space will run out. 
	// Once that happens, we alloc memeory for more rows in the 2D buffer.

	maxRow_mal = 1000;
	rowCap_mal = 10000;
	capacity_mal = maxRow_mal * rowCap_mal;
	curRow_mal = 0;
	curPoint_mal = 0;
	// buffer_mal = (mpz_t**)malloc(sizeof(mpz_t*) * (maxRow_mal * 2));
	curCap_mal = 10;
	curCapacity_mal = curCap_mal * rowCap_mal;

	buffer_mal = (mpz_t **)malloc(sizeof(mpz_t *) * (maxRow_mal * 2));
	mpz_init(r);

	for (int i = 0; i < curCap_mal; i++)
	{
		buffer_mal[2 * i] = (mpz_t *)malloc(sizeof(mpz_t) * (rowCap_mal));
		buffer_mal[2 * i + 1] = (mpz_t *)malloc(sizeof(mpz_t) * (rowCap_mal));
		for (int j = 0; j < rowCap_mal; j++)
		{
			mpz_init(buffer_mal[2 * i][j]);
			mpz_init(buffer_mal[2 * i + 1][j]);
		}
	}

	computeLagrangeWeight_mal();
}

bool MaliciousSMC::checkBuffer(int size)
{

	// printf("check buffer: %d \n", size);
	// printf("curCap_mal :%d; curRow_mal: %d;  curPoint_mal : %d \n", curCap_mal, curRow_mal, curPoint_mal);
	// printf("---current remaining space: %d \n", curCapacity_mal);
	if (curCapacity_mal > size)
	{
		return true;
	}
	if (curCapacity_mal + (maxRow_mal - curCap_mal) * rowCap_mal < size)
		return false;

	printf("--Need more space \n");
	int new_row = 1;

	while (size > new_row * rowCap_mal + curCapacity_mal)
	{
		new_row++;
	}
	for (int i = 0; i < new_row; i++)
	{
		buffer_mal[curCap_mal * 2] = (mpz_t *)malloc(sizeof(mpz_t) * (rowCap_mal));
		buffer_mal[curCap_mal * 2 + 1] = (mpz_t *)malloc(sizeof(mpz_t) * (rowCap_mal));
		for (int j = 0; j < rowCap_mal; j++)
		{
			mpz_init(buffer_mal[curCap_mal * 2][j]);
			mpz_init(buffer_mal[curCap_mal * 2 + 1][j]);
		}
		curCap_mal++;
		curCapacity_mal += rowCap_mal;
	}
	return true;
}
void MaliciousSMC::pushBuffer(mpz_t *result, mpz_t *resultR, int size)
{
	//printf("push into buffer: %d\n", size);
	if (!checkBuffer(size))
	{
		printf("buffer filled, no more varification could be done\n");
		return;
	}
	//  printf("checked \n");
	// check buffer avaiablity and push results into the buffer. This function will be called inside of functions like mult_mal/add_mal
	// first fill unfilled row
	if (size <= rowCap_mal - curPoint_mal)
	{
		for (int i = 0; i < size; i++)
		{
			mpz_set(buffer_mal[curRow_mal * 2][curPoint_mal], result[i]);
			mpz_set(buffer_mal[curRow_mal * 2 + 1][curPoint_mal], resultR[i]);
			curPoint_mal++;
			curCapacity_mal--;
		}
		if (curPoint_mal == rowCap_mal)
		{
			curPoint_mal = curPoint_mal % rowCap_mal;
			curRow_mal++;
		}
		return;
	}

	int sizeP = rowCap_mal - curPoint_mal;
	for (int i = 0; i < sizeP; i++)
	{
		mpz_set(buffer_mal[curRow_mal * 2][curPoint_mal], result[i]);
		mpz_set(buffer_mal[curRow_mal * 2 + 1][curPoint_mal], resultR[i]);
		curPoint_mal++;
		curCapacity_mal--;
	}
	curPoint_mal = curPoint_mal % rowCap_mal;
	curRow_mal++;
	// fill serveral full rows
	int entireRow = (size - sizeP) / rowCap_mal;
	for (int i = 0; i < entireRow; i++)
	{
		for (int j = 0; j < rowCap_mal; j++)
		{
			mpz_set(buffer_mal[curRow_mal * 2][curPoint_mal], result[sizeP]);
			mpz_set(buffer_mal[curRow_mal * 2 + 1][curPoint_mal], resultR[sizeP]);
			curPoint_mal++;
			sizeP++;
			curCapacity_mal--;
		}
		curRow_mal++;
		curPoint_mal = 0;
	}
	// fill the rest into next row
	for (int i = sizeP; i < size; i++)
	{
		mpz_set(buffer_mal[curRow_mal * 2][curPoint_mal], result[i]);
		mpz_set(buffer_mal[curRow_mal * 2 + 1][curPoint_mal], resultR[i]);
		curPoint_mal++;
		curCapacity_mal--;
	}
}

void MaliciousSMC::verify()
{
	struct timeval start;
	struct timeval end;
	unsigned long timerp1;
	gettimeofday(&start, NULL);
	int verifySize = curRow_mal * rowCap_mal + curPoint_mal;
	int pid = net.getID();
	mpz_t field;
	mpz_init(field);
	ss->getFieldSize(field);
	//printf("Verify size: %d, %d, %d\n", curRow_mal, rowCap_mal, curPoint_mal);
	//printf("Verify size: %d\n", verifySize);
	int curP = 0;

	mpz_t **a;
	mpz_t *rArray; // container for randomness i.e., \alpha \beta
	mpz_t *u1;
	mpz_t *u2;
	a = (mpz_t **)malloc(sizeof(mpz_t *) * (2));
	for (int _picco_i = 0; _picco_i < 2; _picco_i++)
	{
		a[_picco_i] = (mpz_t *)malloc(sizeof(mpz_t) * (verifySize));
		for (int _picco_j = 0; _picco_j < verifySize; _picco_j++)
			mpz_init(a[_picco_i][_picco_j]);
	}
	rArray = (mpz_t *)malloc(sizeof(mpz_t) * (verifySize));
	u1 = (mpz_t *)malloc(sizeof(mpz_t) * (verifySize));
	u2 = (mpz_t *)malloc(sizeof(mpz_t) * (verifySize));
	for (int _picco_i = 0; _picco_i < verifySize; _picco_i++)
	{
		mpz_init(rArray[_picco_i]);
		mpz_init(u1[_picco_i]);
		mpz_init(u2[_picco_i]);
	}
	for (int i = 0; i < curRow_mal; i++)
	{
		for (int j = 0; j < rowCap_mal; j++)
		{
			mpz_set(a[0][curP], buffer_mal[i * 2][j]);
			mpz_set(a[1][curP], buffer_mal[i * 2 + 1][j]);
			curP++;
		}
	}
	for (int i = 0; i < curPoint_mal; i++)
	{
		mpz_set(a[0][curP], buffer_mal[curRow_mal * 2][i]);
		mpz_set(a[1][curP], buffer_mal[curRow_mal * 2 + 1][i]);
		curP++;
	}
	// 1 share and open a seed
	// assume the seed for each party is party_id
	mpz_t *randSeed = (mpz_t *)malloc(sizeof(mpz_t) * 1);
	mpz_init(randSeed[0]);
	mpz_set_ui(randSeed[0], pid);

	mpz_t **seedShares = (mpz_t **)malloc(sizeof(mpz_t *) * peers);
	for (int i = 0; i < peers; ++i)
	{
		seedShares[i] = (mpz_t *)malloc(sizeof(mpz_t) * 1);
		for (int j = 0; j < 1; ++j)
			mpz_init(seedShares[i][j]);
	}

	// gmp_printf("%Zd \n", randSeed[0]);
	int openSeed = open(randSeed[0], -1);
	// printf("opened seed: %d", openSeed);
	mpz_set_ui(randSeed[0], openSeed);
	// gmp_printf("%Zd \n", randSeed[0]);

	gmp_randstate_t verifyRandomState;
	gmp_randinit_mt(verifyRandomState);
	gmp_randseed(verifyRandomState, randSeed[0]);

	// generate \alpha \beta
	for (int i = 0; i < verifySize; ++i)
	{
		mpz_urandomm(rArray[i], verifyRandomState, field);
		// gmp_printf("%Zd \n", rArray[i]);
	}

	// local mult
	for (int i = 0; i < verifySize; ++i)
	{
		mpz_mul(u1[i], rArray[i], a[1][i]);
		mpz_mul(u2[i], rArray[i], a[0][i]);
		mpz_mod(u1[i], u1[i], field);
		mpz_mod(u2[i], u2[i], field);
		// printf("u1 :%d \n", smc_open(u1[i], -1));
		// printf("u2 :%d \n", smc_open(u2[i], -1));
	}
	mpz_t sum1;
	mpz_t sum2;
	mpz_init(sum1);
	mpz_init(sum2);
	for (int i = 0; i < verifySize; ++i)
	{
		mpz_add(sum1, sum1, u1[i]);
		mpz_add(sum2, sum2, u2[i]);
		mpz_mod(sum1, sum1, field);
		mpz_mod(sum2, sum2, field);

		// printf("sum1 :%d \n", smc_open(sum1, -1));
		// printf("sum2 :%d \n", smc_open(sum2, -1));
	}
	//  gettimeofday(&start,NULL);
	Mul->doOperation(&sum2, &r, &sum2, 1, -1);
	mpz_sub(sum2, sum2, sum1);
	mpz_mod(sum2, sum2, field);
	int checkResult = open(sum2, -1);
	malBufferReset();

	if(checkResult == 0) {
		// printf("CORRECT! \n");
	} else {
		printf("check res: %d", checkResult);
	}
	
	for (int _picco_i = 0; _picco_i < 2; _picco_i++)
	{
		for (int _picco_j = 0; _picco_j < verifySize; _picco_j++)
			mpz_clear(a[_picco_i][_picco_j]);
		free(a[_picco_i]);
	}
	free(a);
	for (int i = 0; i < verifySize; i++)
	{
		mpz_clear(rArray[i]);
		mpz_clear(u1[i]);
		mpz_clear(u2[i]);
	}
	for (int i = 0; i < peers; ++i)
	{
		mpz_clear(seedShares[i][0]);
		free(seedShares[i]);
	}
	free(seedShares);
	free(rArray);
	free(u1);
	free(u2);
	mpz_clear(randSeed[0]);
	free(randSeed);
	gmp_randclear(verifyRandomState);
	gettimeofday(&end, NULL); // stop timer here p1
	VerificationTimer += 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
}

void MaliciousSMC::malBufferReset()
{
	curRow_mal = 0;
	curPoint_mal = 0;
	curCapacity_mal = curCap_mal * rowCap_mal;
}

void MaliciousSMC::clean_mal()
{
	printf("total verification time is %ld us\n", VerificationTimer);
	printf("Need to clean %d rows \n", curCap_mal);
	mpz_clear(r);

	for (int _picco_i = 0; _picco_i < 2 * curCap_mal; _picco_i++)
	{
		for (int _picco_j = 0; _picco_j < rowCap_mal; _picco_j++)
			mpz_clear(buffer_mal[_picco_i][_picco_j]);
		free(buffer_mal[_picco_i]);
	}
	free(buffer_mal);
	printf("Exit Malicious Mode\n");
}

void MaliciousSMC::computeLagrangeWeight_mal()
{
	mpz_t nom, denom, t1, t2, temp;
	mpz_init(nom);
	mpz_init(denom);
	mpz_init(t1);
	mpz_init(t2);
	mpz_init(temp);

	lagrangeWeight_mal = (mpz_t **)malloc(sizeof(mpz_t *) * peers);

	for (int i = 0; i < peers; i++)
	{
		lagrangeWeight_mal[i] = (mpz_t *)malloc(sizeof(mpz_t) * peers);
		for (int j = 0; j < peers; ++j)
		{
			mpz_init(lagrangeWeight_mal[i][j]);
		}
	}
	for (int peer = 0; peer < peers; peer++)
	{
		for (int count = 1; count <= 2; count++)
		{
			int point = peer + 1;
			mpz_set_ui(nom, 1);
			mpz_set_ui(denom, 1);

			int l = (peer + count) % 3;
			mpz_set_ui(t1, l + 1);
			mpz_set_ui(t2, point);
			ss->modSub(temp, t1, t2);
			ss->modInv(denom, temp);

			ss->modMul(lagrangeWeight_mal[peer][l], denom, t1);
			// gmp_printf ("lagrangeWeight of [%d][%d] is %Zd\n",  peer, l, lagrangeWeight_mal[peer][l]);
		}
	}

	mpz_clear(nom);
	mpz_clear(denom);
	mpz_clear(t1);
	mpz_clear(t2);
	mpz_clear(temp);
}

void MaliciousSMC::reconstructSecretCheck(mpz_t *result, mpz_t **y, int size, bool isMultiply)
{
	mpz_t temp;
	mpz_init(temp);
	mpz_t preResult;
	mpz_init(preResult);

	for (int i = 0; i < size; i++)
		mpz_set_ui(result[i], 0);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < peers; ++j)
		{
			int curPoint = (j - 1 + peers) % peers;
			int nextPoint = (j + 1) % peers;
			ss->modMul(temp, y[curPoint][i], lagrangeWeight_mal[curPoint][nextPoint]);
			ss->modAdd(preResult, preResult, temp);
			ss->modMul(temp, y[nextPoint][i], lagrangeWeight_mal[nextPoint][curPoint]);
			ss->modAdd(preResult, preResult, temp);
			if (j != 0)
			{
				if (mpz_cmp(preResult, result[i]) != 0)
				{
				}
				// printf("open incorrect \n");
			}
			mpz_set(result[i], preResult);
			mpz_set_ui(preResult, 0);
			// gmp_printf ("result at is %Zd\n", result[i]);
		}
	}
	mpz_clear(temp);
	mpz_clear(preResult);
}


int MaliciousSMC::open(mpz_t var, int threadID)
{
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
	// gmp_printf ("temp at is %Zd\n", tmp);
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