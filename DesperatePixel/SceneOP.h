#pragma once
#ifndef SCENEOP_H
#define SCENEOP_H

#include "GameScene.h"
#include "GameScrn.h"
#include <vector>

class SceneOP :public GameScene
{
public:
	SceneOP();
	~SceneOP();

	void Init(int iInitVal = 0);
	void End() {}
	void OnResetDevice() {}
	void OnLostDevice() {}
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
	void Refresh() {}

private:
	RECT	m_rcLogo;
	std::vector<PIX> m_vecLogo;

	int		m_iCounter;
	float	m_fTimer;
	bool	m_bKeyUp;
	bool	m_bSceneOver;
};

#endif // !SCENEOP_H
