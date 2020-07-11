/*
   RainbowCrack - a general propose implementation of Philippe Oechslin's faster time-memory trade-off technique.

   Copyright (C) Zhu Shuanglei <shuanglei@hotmail.com>
*/

#include "HashRoutine.h"
#include "sha1.h"

#define MIN_HASH_LEN    8
#define MAX_HASH_LEN    32

void Audible(
    unsigned char *pData,
    int  uLen,
    unsigned char Hash[MAX_HASH_LEN]);


//////////////////////////////////////////////////////////////////////

CHashRoutine::CHashRoutine()
{
	// Notice: MIN_HASH_LEN <= nHashLen <= MAX_HASH_LEN

	AddHashRoutine("audible", Audible, 20);
}

CHashRoutine::~CHashRoutine()
{
}

void CHashRoutine::AddHashRoutine(string sHashRoutineName, HASHROUTINE pHashRoutine, int nHashLen)
{
	vHashRoutineName.push_back(sHashRoutineName);
	vHashRoutine.push_back(pHashRoutine);
	vHashLen.push_back(nHashLen);
}

string CHashRoutine::GetAllHashRoutineName()
{
	string sRet;
	int i;
	for (i = 0; i < vHashRoutineName.size(); i++)
		sRet += vHashRoutineName[i] + " ";

	return sRet;
}

void CHashRoutine::GetHashRoutine(string sHashRoutineName, HASHROUTINE& pHashRoutine, int& nHashLen)
{
	int i;
	for (i = 0; i < vHashRoutineName.size(); i++)
	{
		if (sHashRoutineName == vHashRoutineName[i])
		{
			pHashRoutine = vHashRoutine[i];
			nHashLen = vHashLen[i];
			return;
		}
	}

	pHashRoutine = NULL;
	nHashLen = 0;
}


/*
 * http://www.tobtu.com/rtcalc.php#params
 *
 * keyspace is 256^4 (length is always 4)
 * 99.999999% (Total success rate)
 *
 * ./rtgen audible byte 4 4 0 10000 10008356 0
 * ./rtgen audible byte 4 4 1 10000 10008356 0
 * ./rtgen audible byte 4 4 2 10000 10008356 0
 * ./rtgen audible byte 4 4 3 10000 10008356 0
 * ./rtgen audible byte 4 4 4 10000 10008356 0
 * ./rtgen audible byte 4 4 5 10000 10008356 0
 * ./rtgen audible byte 4 4 6 10000 10008356 0
 * ./rtgen audible byte 4 4 7 10000 10008356 0
 * ./rtgen audible byte 4 4 8 10000 10008356 0
 * ./rtgen audible byte 4 4 9 10000 10008356 0
 *
 * ./rtsort *.rt
 * ./rt2rtc *.rt 21 24 -m 18 -p
 * ./rt2rtc *.rt 25 25 -m 512 -p
 */


void
Audible(
    unsigned char *pData,               // [in] plaintext to be hashed
    int  uLen,                 // [in] length of the plaintext
    unsigned char Hash[MAX_HASH_LEN])   // [out] the result hash, size of the buffer is MAX_HASH_LEN bytes
{
    unsigned char fixed_key[] = { 0x77, 0x21, 0x4d, 0x4b, 0x19, 0x6a, 0x87,
        0xcd, 0x52, 0x00, 0x45, 0xfd, 0x20, 0xa5, 0x1d, 0x67 };

    unsigned char intermediate_key[20] = {0};
    unsigned char intermediate_iv[20] = {0};
    SHA_CTX ctx;

    SHA1_Init(&ctx);
    SHA1_Update(&ctx, fixed_key, 16);
    SHA1_Update(&ctx, pData, uLen);
    SHA1_Final(intermediate_key, &ctx);

    SHA1_Init(&ctx);
    SHA1_Update(&ctx, fixed_key, 16);
    SHA1_Update(&ctx, intermediate_key, 20);
    SHA1_Update(&ctx, pData, uLen);
    SHA1_Final(intermediate_iv, &ctx);

    SHA1_Init(&ctx);
    SHA1_Update(&ctx, intermediate_key, 16);
    SHA1_Update(&ctx, intermediate_iv, 16);
    SHA1_Final(Hash, &ctx);
}

