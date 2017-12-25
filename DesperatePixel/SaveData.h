#pragma once
#ifndef SAVEDATA_H
#define SAVEDATA_H

#include "DXUT.h"

struct SaveData
{
	int _iStage[4];
	int _iEnd[3];
};

SaveData &GetSaveData();
BOOL InitSaveData();
BOOL ReadSaveData();
BOOL WriteSaveData();

#endif // !SAVEDATA_H
