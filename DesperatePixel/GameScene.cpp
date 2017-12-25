#include "GameScene.h"
#include "Settings.h"

void SetPixRect(LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom)
{
	int iPixSize = GetSettings()._iPixSize;
	SetRect(lprc, xLeft * iPixSize, yTop * iPixSize, xRight * iPixSize, yBottom * iPixSize);
}

void SetGridRect(LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom, int iGridSize)
{
	SetRect(lprc, xLeft * iGridSize, yTop * iGridSize, xRight * iGridSize, yBottom * iGridSize);
}

void ExpandRect(LPRECT lprc, int iSize)
{
	lprc->left -= iSize;
	lprc->top -= iSize;
	lprc->right += iSize;
	lprc->bottom += iSize;
}

void ShrinkRect(LPRECT lprc, int iSize)
{
	lprc->left += iSize;
	lprc->top += iSize;
	lprc->right -= iSize;
	lprc->bottom -= iSize;
}