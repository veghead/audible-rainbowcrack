/*
   RainbowCrack - a general propose implementation of Philippe Oechslin's faster time-memory trade-off technique.

   Copyright (C) Zhu Shuanglei <shuanglei@hotmail.com>
*/

#include "CrackEngine.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define APP_NAME "audible-crack"

//////////////////////////////////////////////////////////////////////

void GetTableList(int argc, char* argv[], vector<string>& vPathName)
{
	vPathName.clear();

	int i;
	for (i = 1; i <= argc - 2; i++)
	{
		string sPathName = argv[i];

		struct stat buf;
		if (lstat(sPathName.c_str(), &buf) == 0)
		{
			if (S_ISREG(buf.st_mode))
				vPathName.push_back(sPathName);
		}
	}
}

bool NormalizeHash(string& sHash)
{
	string sNormalizedHash = sHash;

	if (   sNormalizedHash.size() % 2 != 0
		|| sNormalizedHash.size() < MIN_HASH_LEN * 2
		|| sNormalizedHash.size() > MAX_HASH_LEN * 2)
		return false;

	// Make lower
	int i;
	for (i = 0; i < sNormalizedHash.size(); i++)
	{
		if (sNormalizedHash[i] >= 'A' && sNormalizedHash[i] <= 'F')
			sNormalizedHash[i] = sNormalizedHash[i] - 'A' + 'a';
	}

	// Character check
	for (i = 0; i < sNormalizedHash.size(); i++)
	{
		if (   (sNormalizedHash[i] < 'a' || sNormalizedHash[i] > 'f')
			&& (sNormalizedHash[i] < '0' || sNormalizedHash[i] > '9'))
			return false;
	}

	sHash = sNormalizedHash;
	return true;
}

void Usage(char *name)
{
	printf("%s - based on:\n", name);
	Logo();

	printf("usage: %s <rainbow_table_pathname> <hash>\n", name);
	printf("rainbow_table_pathname: pathname of the rainbow table(s), (only 'rt' format supported)\n");
	printf("\n");
	printf("example: %s audible-tables/*.rt 5d41402abc4b2a76b9719d911017c5f92df87f20\n", name);
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		Usage(argv[0]);
		return 0;
	}
	string sInput			= argv[argc - 1];

	// vPathName
	vector<string> vPathName;
	GetTableList(argc, argv, vPathName);
	if (vPathName.size() == 0)
	{
		printf("no rainbow table found\n");
		return 0;
	}

	// fCrackerType, vHash, vUserName, vLMHash
	bool fCrackerType;			// true: hash cracker, false: lm cracker
	vector<string> vHash;		// hash cracker
	vector<string> vUserName;	// lm cracker
	vector<string> vLMHash;		// lm cracker
	vector<string> vNTLMHash;	// lm cracker
	fCrackerType = true;

	string sHash = sInput;
	if (NormalizeHash(sHash)) {
		vHash.push_back(sHash);
	} else {
		printf("invalid hash: %s\n", sHash.c_str());
	}
	
	// hs
	CHashSet hs;
	if (fCrackerType)
	{
		int i;
		for (i = 0; i < vHash.size(); i++)
			hs.AddHash(vHash[i]);
	}
	CCrackEngine ce;
	ce.Run(vPathName, hs);

	// Statistics
	printf("statistics\n");
	printf("-------------------------------------------------------\n");
	printf("plaintext found:		  %d of %d (%.2f%%)\n", hs.GetStatHashFound(),
															hs.GetStatHashTotal(),
															100.0f * hs.GetStatHashFound() / hs.GetStatHashTotal());
	printf("total disk access time:   %.2f s\n", ce.GetStatTotalDiskAccessTime());
	printf("total cryptanalysis time: %.2f s\n", ce.GetStatTotalCryptanalysisTime());
	printf("total chain walk step:	%d\n",	 ce.GetStatTotalChainWalkStep());
	printf("total false alarm:		%d\n",	 ce.GetStatTotalFalseAlarm());
	printf("total chain walk step due to false alarm: %d\n", ce.GetStatTotalChainWalkStepDueToFalseAlarm());
	printf("\n");

	// Result
	printf("result\n");
	printf("-------------------------------------------------------\n");
	if (fCrackerType)
	{
		int i;
		for (i = 0; i < vHash.size(); i++)
		{
			string sPlain, sBinary;
			if (!hs.GetPlain(vHash[i], sPlain, sBinary))
			{
				sPlain  = "<notfound>";
				sBinary = "<notfound>";
			}

			printf("%s  %s  hex:%s\n", vHash[i].c_str(), sPlain.c_str(), sBinary.c_str());
		}
	}

	return 0;
}
