#pragma once
#ifndef SCENESNAKE_H
#define SCENESNAKE_H

#include "GameScene.h"
#include "GameScrn.h"
#include "GameAudio.h"
#include <vector>

class SceneSnake :public GameScene
{
public:
	SceneSnake();
	~SceneSnake();

	void Init(int iInitVal = 1);
	void End();
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
	void Refresh();

private:
	struct Snake
	{
		std::vector<POINT> _vecBody;
		D3DXCOLOR _color = WHITE;
		int _iDir = 0;
	};

private:
	void InitField();
	void InitStage();

	void SetValid(bool bValid, POINT point);
	bool IsValid(POINT point);

	bool NewEnemy();
	bool NewFood();
	bool NewPoison();
	bool Move(Snake *pSnake, int iDir);
	void Show(Snake *pSnake);

private:
	//GameState

	bool m_bSceneOver;
	bool m_bFieldInit;
	bool m_bStageInit;
	bool m_bStageShow;
	bool m_bPlay;
	bool m_bBeginShow; //Beginning story
	bool m_bEndShow;
	bool m_bNextSent;
	bool m_bWin;
	bool m_bLose;
	bool m_bEnding;
	float m_timeShow;

	int m_iSize;
	int m_pxField;
	int m_pyField;
	int m_cxField;
	int m_cyField;
	int m_iTarScore;
	float m_timePeriod;

	//Counter

	int m_iStage;
	int m_iScore;
	int m_iScoreBlack;
	float m_timeUpdate;
	float m_timeEnemyUpdate;
	int m_iSent;
	int m_iStageOffset;
	int m_pxEnemy;
	int m_pxFriend;

	//Control

	bool m_bKeyUp[12];
	float m_fKeyUp[12];
	float m_fRate;
	int m_iMajorCtrl;
	int m_iMinorCtrl;
	
	//Snake
	
	Snake m_sWhite;
	Snake m_sBlack;

	std::vector<Snake> m_sEnemy;

	//Map

	D3DXCOLOR m_colorMap[4] = { HUNTER, MAGE, SHAMAN, WARRIOR };
	int m_iColorField[CXPIX][CYPIX];

	std::vector<POINT> m_vecFood;
	std::vector<POINT> m_vecPoison;

	std::vector<POINT> m_vecValid;

	//Display

	bool m_bRefresh;
	bool m_bHighFPS;

	RECT m_rcField;
	RECT m_rcNext;

	RECT m_rcScreen;
	RECT m_rcTextTop;
	RECT m_rcTextBottom;

	std::vector<ID3DXFont*> m_vecFont = { NULL, NULL };

	//Sound

	Sound *m_pBGM = NULL;
	Sound *m_pSE = NULL;

	int m_iCurBGM;
	bool m_bFade;
	float m_timeFade;

	//Text

	typedef std::vector<LPCWSTR> VECLPCWSTR;
	typedef std::vector<VECLPCWSTR> STORY;

	std::vector<STORY> m_vecBegin =
	{
		//stage 1
		{
			{ L"This time, all start from the smallest world...",
			L"��Σ�һ�д���С�����翪ʼ����" },
			{ L"Stage 1 - A Small World",
			L"��1�� - ��С������" },
			{ L"You need to move \n (Arrow Keys or WSAD), \n or...",
			L"����Ҫ������(�������WSAD)\n\n���򡭡�" },
			{ L"Get the foods, strenthen youself,\n and fullfill the world.",
			L"��֮����ȡʳ������ǿ���Լ�\n\nֱ��ռ������������" }
		},
		//stage 2
		{
			{ L"Stage 2 - Growth",
			L"��2�� - �ɳ�" },
			{ L"You can see a wider world,\n but still surrounded by walls.",
			L"����Կ�����������������\n\n����Ȼ��ǽ�ڷ����" },
			{ L"Become strong enough,\n to be able to break them.",
			L"����㹻ǿ��ȥ��������" },
			{ L"From now on, once you start moving,\n you will no longer be able to stop.",
			L"�����ڿ�ʼ��һ���㿪ʼ�ж�\n\n�㽫������ͣ����" },
			{ L"If you want, accelerate youself\n (Ctrl Key)",
			L"�����Ҫ�Ļ���Ҳ���Լ���(Ctrl��)" },
		},
		//stage 3
		{
			{ L"Stage 3 - Mess",
			L"��3�� - ����" },
			{ L"The foods are limited now.",
			L"ʳ�￪ʼ���������" },
			{ L"Fortunately, the amount is still enough\n for you to breakout.",
			L"���˵��ǣ�����������㹻��ɳ����ܻ���ǽ��" },
			{ L"But be careful,\n the poison can kill you.",
			L"����С�ģ���Щ���ؿ���ɱ����" },
		},
		//stage 4
		{
			{ L"Stage 4 - Friend",
			L"��4�� - ����" },
			{ L"Seems you have got a companion.",
			L"������������һ������" },
			{ L"However, the amount of food\n is not enough for you to breakout.",
			L"Ȼ��ʳ��������ƺ���������ȥ����ǽ����" },
			{ L"...",
			L"����" },
			{ L"I've got an idea...",
			L"������һ�����⡭��" },
			{ L"Just balance the strenth of you two.",
			L"ƽ��������������" },
			{ L"I will tell you what to do later.",
			L"֮�����ٸ�������ô��" },
		},
		//stage 5
		{
			{ L"His death attracts the enemies outside.",
			L"��������������ǽ��ĵ���" },
			{ L"They break the wall and find you...",
			L"���Ǵ�����ǽ�ڷ�������" },
			{ L"Stage 5 - Fight to Live",
			L"��5�� - ս��������" },
			{ L"The world is full of combat.",
			L"������罫��������" },
			{ L"Fortunately, you are able to fight them.",
			L"���ң����ڵ�������������һս" },
			{ L"Just become stronger than them, \nthen destory and eat them.",
			L"��ñ����Ǹ�ǿ����������������" }
		},
		//stage 6
		{
			{ L"Stage 6 - Self",
			L"��6�� - һ��" },
			{ L"You are strong enough now.",
			L"���Ѿ��㹻ǿ����" },
			{ L"Even the strongest enemy \nis just a piece of cake to you.",
			L"��ʹ������ǿ�ĵ��˶���Ҳ������С��һ��" },
			{ L"Just fullfill the world \nand reach the outer world.",
			L"��֮�����һ��ռ�����������" },
		},
		//stage 7
		{
			{ L"Wow, congratulations! \nA big world... with no boundary!",
			L"��Ŷ����ϲ�㣡\n\nһ��û�б߼ʵĹ�������" },
			{ L"Wait, what the...?",
			L"�ȵȣ����ǡ�����" },
			{ L"Is that... an enemy?",
			L"����һ������������" },
			{ L"We are too small to fight him...",
			L"����̫����С��ȫ�޷��Կ�����" },
			{ L"And nowhere to run...",
			L"�����޴����ӡ���" }
		},
		//stage 5 + 3
		{
			{ L"What the hell have you done!",
			L"�㶼����Щʲô��" },
			{ L"Your death attracts the enemies outside.",
			L"�������������ǽ��ĵ���" },
			{ L"They break the wall and find you.",
			L"���Ǵ�����ǽ�ڷ�������" },
			{ L"And your friend is missing...",
			L"���������Ҳ���䲻������" },
			{ L"Stage 5 - Fight to Live",
			L"��5�� - ս��������" },
			{ L"I resurrected you this time, \nnever again!",
			L"����Ҹ������㣬���������´���" },
			{ L"Now, become stronger, \nthen destory and eat them.",
			L"���ڣ���ø�ǿ�����������ɵ���" }
		},
		//stage 6 + 3
		{
			{ L"Stage 6 - Self",
			L"��6�� - һ��" },
			{ L"You are strong enough now.",
			L"���Ѿ��㹻ǿ����" },
			{ L"Even the strongest enemy \nis just a piece of cake to you.",
			L"��ʹ������ǿ�ĵ��˶���Ҳ������С��һ��" },
			{ L"Just fullfill the world \nand reach the outer world.",
			L"��֮�����һ��ռ�����������" },
		},
		//stage 7 + 3
		{
			{ L"Wow, congratulations! \nA big world... with no boundary!",
			L"��Ŷ����ϲ�㣡\n\nһ��û�б߼ʵĹ�������" },
			{ L"Wait, what the...?",
			L"�ȵȣ����ǡ�����" },
			{ L"Is that... an enemy?",
			L"����һ������������" },
			{ L"We are too small to fight him...",
			L"����̫����С��ȫ�޷��Կ�����" },
			{ L"And nowhere to run...",
			L"�����޴����ӡ���" },
			{ L"Oh god... Another one?",
			L"���ġ�������һ����" },
			{ L"Bad luck. Maybe time to say goodbye.",
			L"��������ʱ��˵�ټ���" },
			{ L"...?",
			L"����" },
			{ L"He is... trying to protect you...?",
			L"���ƺ��ڡ�����ͼ�����㣿" },
			{ L"Chance! Run !Now!",
			L"�û��ᣬ���ӣ�" }
		},
	};

	std::vector<VECLPCWSTR> m_vecEnd =
	{
		//stage 1
		{ L"Ending 0 - Stay in the Small World",
		L"���0 - ������С������" },
		//stage 2
		{ L"Now break the wall. \nDon't be afraid, just go straight towards it.",
		L"���ڣ�����ǽ��\n\n���غ��£�ֻҪ������������" },
		//stage 3
		{ L"Now do that again,\nyou will be able to get out.",
		L"���ڣ��ٴδ���ǽ�ڵ������" },
		//stage 4
		{ L"Now, I've controlled him. \nJust watch him die.",
		L"���ڣ��ҿ�����������������������������" },
		//stage 5
		{ L"You can get to the outer world now.",
		L"�����ǰ��������������" },
		//stage 6
		{ L"",
		L"" },
		//stage 7
		{ L"Ending 1 - Despair",
		L"���1 - ����" },
		//stage 5 + 3
		{ L"You can get to the outer world now.",
		L"�����ǰ��������������" },
		//stage 6 + 3
		{ L"",
		L"" },
		//stage 7 + 3
		{ L"Ending 2 - Protected",
		L"���2 - ����" }
	};
};

#endif // !SCENESNAKE_H
