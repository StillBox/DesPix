#include "SaveData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <shlobj.h>
#include <shlwapi.h>
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")

SaveData *g_pSaveData = new SaveData();
bool g_bSaveDataUpdated;

SaveData &GetSaveData()
{
	assert(g_pSaveData != NULL);
	return *g_pSaveData;
}

BOOL InitSaveData()
{
	for (int i = 0; i < 4; i++)
		GetSaveData()._iStage[i] = 1;

	for (int i = 0; i < 3; i++)
		GetSaveData()._iEnd[i] = 0;

	ReadSaveData();
	WriteSaveData();

	return TRUE;
}

BOOL ReadSaveData()
{
	WCHAR szPath[MAX_PATH] = L"";
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	wcscat_s(szPath, L"\\StillBox");
	wcscat_s(szPath, L"\\DesPix");
	wcscat_s(szPath, L"\\Save.dat");
	
	std::ifstream infile;
	infile.open(szPath, std::ios::_Nocreate);

	if (!infile.is_open())
	{
		infile.close();
		return FALSE;
	}
	else
	{
		std::string szIn, szData;
		infile >> szIn;

		int iSeed, iMul, iData;
		std::istringstream inStr;

		szData = szIn.substr(0, 2);
		szIn = szIn.substr(2);
		inStr.str(szData);
		inStr >> iSeed;
		inStr.clear();

		for (int i = 0; i < 4; i++)
		{
			szData = szIn.substr(0, 2);
			szIn = szIn.substr(2);
			inStr.str(szData);
			inStr >> iMul;
			inStr.clear();

			szData = szIn.substr(0, 4);
			szIn = szIn.substr(4);
			inStr.str(szData);
			inStr >> iData;
			inStr.clear();

			g_pSaveData->_iStage[i] = (10000 + iData - iSeed * iMul) % 10000;
		}

		for (int i = 0; i < 3; i++)
		{
			szData = szIn.substr(0, 2);
			szIn = szIn.substr(2);
			inStr.str(szData);
			inStr >> iMul;
			inStr.clear();

			szData = szIn.substr(0, 4);
			szIn = szIn.substr(4);
			inStr.str(szData);
			inStr >> iData;
			inStr.clear();

			g_pSaveData->_iEnd[i] = (10000 + iData - iSeed * iMul) % 10000;
		}
	}

	infile.close();
	return TRUE;
}

BOOL WriteSaveData()
{
	WCHAR szPath[MAX_PATH] = L"";
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	wcscat_s(szPath, L"\\StillBox");
	if (!PathFileExists(szPath))
		CreateDirectory(szPath, NULL);
	wcscat_s(szPath, L"\\DesPix");
	if (!PathFileExists(szPath))
		CreateDirectory(szPath, NULL);
	wcscat_s(szPath, L"\\Save.dat");

	std::ofstream outfile(szPath);

	int iSeed = 11 + rand() % 89;
	int iMul, iOut;

	outfile << iSeed;

	for (int i = 0; i < 4; i++)
	{
		iMul = 11 + rand() % 89;
		outfile << iMul;
		iOut = (iSeed * iMul + g_pSaveData->_iStage[i]) % 10000;
		outfile << std::setw(4) << std::setfill('0') << iOut;
	}

	for (int i = 0; i < 3; i++)
	{
		iMul = 11 + rand() % 89;
		outfile << iMul;
		iOut = (iSeed * iMul + g_pSaveData->_iEnd[i]) % 10000;
		outfile << std::setw(4) << std::setfill('0') << iOut;
	}

	outfile.close();
	return TRUE;
}