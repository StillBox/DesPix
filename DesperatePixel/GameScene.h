#pragma once
#ifndef GAMESCENE_H
#define GAMESCENE_H

#define EPSILON	0.002f

#include <Windows.h>

class GameScene
{
public:
	GameScene() = default;
	virtual ~GameScene() = default;

	virtual void Init(int iInitVal = 1) = 0;
	virtual void End() = 0;
	virtual void OnResetDevice() = 0;
	virtual void OnLostDevice() = 0;
	virtual void Update(double fTime, float fElapsedTime) = 0;
	virtual void Render(double fTime, float fElapsedTime) = 0;
	virtual void Refresh() = 0;
};

void SetPixRect(LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom);
void SetGridRect(LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom, int iGridSize);
void ExpandRect(LPRECT lprc, int iSize);
void ShrinkRect(LPRECT lprc, int iSize);

#endif // !GAMESCENE_H
