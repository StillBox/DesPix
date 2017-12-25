#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include "DXUT.h"
#include "Game.h"

struct Settings
{
	//Audio
	int _iSoundOn	= 1;
	int _iBGMOn		= 1;
	int _iBGMVol	= 10;
	int _iSEOn		= 1;
	int _iSEVol		= 10;

	//Video
	int _iPixSize	= 4;
	int _iFrame		= 1;
	int _iTexture	= 1;
	int _iShowFPS	= 0;
	int _iHighFPS	= 0;
	int _iContinuous = 0;

	int _cxWindow	= 0;
	int _cyWindow	= 0;
	int _cxDevice	= 0;
	int _cyDevice	= 0;
	
	//Other
	int _iLanguage	= 0;
	int _iSkipGuide = 0;
};

Settings &GetSettings();
BOOL InitSettings();
BOOL SaveSettings();
BOOL UpdateSettings();

#endif // !SETTINGS_H
