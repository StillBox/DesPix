#include "Settings.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <shlobj.h>
#include <shlwapi.h>
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")

Settings *g_pSettings = new Settings();

Settings &GetSettings()
{
	assert(g_pSettings != NULL);
	return *g_pSettings;
}

void DeleteSpace(std::string &str)
{
	int index = 0;
	while (std::string::npos != (index = str.find(' ')))
	{
		str.erase(index, 1);
	}
}

BOOL InitSettings()
{
	//Read Setting File

	WCHAR szPath[MAX_PATH] = L"";
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	wcscat_s(szPath, L"\\StillBox");
	wcscat_s(szPath, L"\\DesPix");
	wcscat_s(szPath, L"\\Settings.ini");

	std::ifstream infile(szPath);
	std::string line;
	while (std::getline(infile, line))
	{
		auto iter = line.find('=');
		if (std::string::npos != iter)
		{
			std::string parameter = line.substr(0, iter);
			std::string value = line.substr(iter + 1);
			DeleteSpace(parameter);
			DeleteSpace(value);
			std::istringstream inStr(value);
			if (0 == parameter.compare("SoundOn"))
				inStr >> g_pSettings->_iSoundOn;
			else if (0 == parameter.compare("BGMOn"))
				inStr >> g_pSettings->_iBGMOn;
			else if (0 == parameter.compare("BGMVol"))
				inStr >> g_pSettings->_iBGMVol;
			else if (0 == parameter.compare("SEOn"))
				inStr >> g_pSettings->_iSEOn;
			else if (0 == parameter.compare("SEVol"))
				inStr >> g_pSettings->_iSEVol; 
			else if (0 == parameter.compare("PixSize"))
				inStr >> g_pSettings->_iPixSize;
			else if (0 == parameter.compare("Frame"))
				inStr >> g_pSettings->_iFrame;
			else if (0 == parameter.compare("Texture"))
				inStr >> g_pSettings->_iTexture;
			else if (0 == parameter.compare("ShowFPS"))
				inStr >> g_pSettings->_iShowFPS;
			else if (0 == parameter.compare("HighFPS"))
				inStr >> g_pSettings->_iHighFPS;
			else if (0 == parameter.compare("Continuous"))
				inStr >> g_pSettings->_iContinuous;
			else if (0 == parameter.compare("Language"))
				inStr >> g_pSettings->_iLanguage;
			else if (0 == parameter.compare("SkipGuide"))
				inStr >> g_pSettings->_iSkipGuide;
		}
	}
	infile.close();

	SaveSettings();

	//Initialization of other data

	g_pSettings->_cxWindow = g_pSettings->_iPixSize * CXPIX;
	g_pSettings->_cyWindow = g_pSettings->_iPixSize * CYPIX;

	g_pSettings->_cxDevice = g_pSettings->_iPixSize * CXPIX;
	g_pSettings->_cyDevice = g_pSettings->_iPixSize * CYPIX;

	return TRUE;
}

BOOL UpdateSettings()
{
	g_pSettings->_cxWindow = g_pSettings->_iPixSize * CXPIX;
	g_pSettings->_cyWindow = g_pSettings->_iPixSize * CYPIX;

	g_pSettings->_cxDevice = g_pSettings->_iPixSize * CXPIX;
	g_pSettings->_cyDevice = g_pSettings->_iPixSize * CYPIX;

	return TRUE;
}

BOOL SaveSettings()
{
	WCHAR szPath[MAX_PATH] = L"";
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	wcscat_s(szPath, L"\\StillBox");
	if (!PathFileExists(szPath))
		CreateDirectory(szPath, NULL);
	wcscat_s(szPath, L"\\DesPix");
	if (!PathFileExists(szPath))
		CreateDirectory(szPath, NULL);
	wcscat_s(szPath, L"\\Settings.ini");
	
	std::ofstream outfile(szPath);

	outfile << "[Audio Setting]\n" << std::endl;

	outfile << "SoundOn = "		<< g_pSettings->_iSoundOn << std::endl;
	outfile << "BGMOn = "		<< g_pSettings->_iBGMOn << std::endl;
	outfile << "BGMVol = "		<< g_pSettings->_iBGMVol << std::endl;
	outfile << "SEOn = "		<< g_pSettings->_iSEOn << std::endl;
	outfile << "SEVol = "		<< g_pSettings->_iSEVol << std::endl;

	outfile << "\n[Video Setting]\n" << std::endl;

	outfile << "PixSize = "		<< g_pSettings->_iPixSize << std::endl;
	outfile << "Frame = "		<< g_pSettings->_iFrame << std::endl;
	outfile << "Texture = "		<< g_pSettings->_iTexture << std::endl;
	outfile << "ShowFPS = "		<< g_pSettings->_iShowFPS << std::endl;
	outfile << "HighFPS = "		<< g_pSettings->_iHighFPS << std::endl;
	outfile << "Continuous = "	<< g_pSettings->_iContinuous << std::endl;

	outfile << "\n[Other]\n" << std::endl;

	outfile << "Language = "	<< g_pSettings->_iLanguage << std::endl;
	outfile << "SkipGuide = "	<< g_pSettings->_iSkipGuide << std::endl;

	outfile.close();

	return TRUE;
}