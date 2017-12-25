#pragma once
#ifndef SCENEEXTRA_H
#define SCENEEXTRA_H

#include "GameScene.h"
#include "GameScrn.h"
#include "GameAudio.h"

class SceneExtra :public GameScene
{
public:
	SceneExtra();
	~SceneExtra();

	void Init(int iInitVal = 1);
	void End();
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
	void Refresh() {}

private:
	void SetButton(int index);
	void SetAlterEgo();

private:
	bool m_bPlay;
	int m_iPlay;
	int m_iFocus;
	bool m_bChange;

	float m_fBeat;
	float m_fTimer;

	bool m_bKeyUp[8];
	float m_fKeyUp[8];

	RECT m_rcButton;

	Sound *m_pBGM = NULL;
	Sound *m_pSE = NULL;

	ID3DXFont *m_pFont = NULL;

	bool m_bListShow;
	int m_nList;
	std::vector<LPCWSTR> m_vecList =
	{
		L"Beginning",
		L"PONG!",
		L"Speed Up",
		L"Execute",
		L"Lonely",
		L"Growth",
		L"Pain",
		L"Smlie",
		L"Stop!",
		L"Struggle",
		L"Lonely II",
		L"Despair"
	};
	std::vector<float> m_vecSPB =
	{
		281.25f,
		125.0f,
		125.0f,
		125.0f,
		375.0f,
		333.33f,
		300.0f,
		300.0f,
		200.0f,
		200.0f,
		300.0f,
		333.33f
	};

	//ALTER EGO

	typedef std::vector<PIX> VECPIX;

	RECT m_rcAE;
	std::vector<VECPIX> m_vecAE;
	std::vector<int> m_vecAEIndex;
	float m_timeUpdateAE;
	int m_iAEIndex;
};

#endif // !SCENEEXTRA_H
