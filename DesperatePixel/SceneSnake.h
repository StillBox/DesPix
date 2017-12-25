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
			L"这次，一切从最小的世界开始……" },
			{ L"Stage 1 - A Small World",
			L"第1关 - 渺小的世界" },
			{ L"You need to move \n (Arrow Keys or WSAD), \n or...",
			L"你需要动起来(方向键或WSAD)\n\n否则……" },
			{ L"Get the foods, strenthen youself,\n and fullfill the world.",
			L"总之，获取食物来增强你自己\n\n直到占据这整个世界" }
		},
		//stage 2
		{
			{ L"Stage 2 - Growth",
			L"第2关 - 成长" },
			{ L"You can see a wider world,\n but still surrounded by walls.",
			L"你可以看到更广阔的世界了\n\n但仍然被墙壁封闭着" },
			{ L"Become strong enough,\n to be able to break them.",
			L"变得足够强大去打破他们" },
			{ L"From now on, once you start moving,\n you will no longer be able to stop.",
			L"从现在开始，一旦你开始行动\n\n你将不再能停下来" },
			{ L"If you want, accelerate youself\n (Ctrl Key)",
			L"如果需要的话你也可以加速(Ctrl键)" },
		},
		//stage 3
		{
			{ L"Stage 3 - Mess",
			L"第3关 - 混乱" },
			{ L"The foods are limited now.",
			L"食物开始变得有限了" },
			{ L"Fortunately, the amount is still enough\n for you to breakout.",
			L"幸运的是，这里的数量足够你成长到能击破墙壁" },
			{ L"But be careful,\n the poison can kill you.",
			L"不过小心，那些毒素可以杀死你" },
		},
		//stage 4
		{
			{ L"Stage 4 - Friend",
			L"第4关 - 朋友" },
			{ L"Seems you have got a companion.",
			L"看起来你有了一个伴呢" },
			{ L"However, the amount of food\n is not enough for you to breakout.",
			L"然而食物的数量似乎不够让你去击破墙壁了" },
			{ L"...",
			L"……" },
			{ L"I've got an idea...",
			L"我有了一个主意……" },
			{ L"Just balance the strenth of you two.",
			L"平衡你们俩的力量" },
			{ L"I will tell you what to do later.",
			L"之后我再告诉你怎么做" },
		},
		//stage 5
		{
			{ L"His death attracts the enemies outside.",
			L"他的死亡吸引了墙外的敌人" },
			{ L"They break the wall and find you...",
			L"他们打破了墙壁发现了你" },
			{ L"Stage 5 - Fight to Live",
			L"第5关 - 战斗与生存" },
			{ L"The world is full of combat.",
			L"这个世界将充满斗争" },
			{ L"Fortunately, you are able to fight them.",
			L"所幸，现在的你足以与他们一战" },
			{ L"Just become stronger than them, \nthen destory and eat them.",
			L"变得比他们更强，击溃并吞噬他们" }
		},
		//stage 6
		{
			{ L"Stage 6 - Self",
			L"第6关 - 一人" },
			{ L"You are strong enough now.",
			L"你已经足够强大了" },
			{ L"Even the strongest enemy \nis just a piece of cake to you.",
			L"即使这里最强的敌人对你也不过是小菜一碟" },
			{ L"Just fullfill the world \nand reach the outer world.",
			L"总之像最初一样占据整个世界吧" },
		},
		//stage 7
		{
			{ L"Wow, congratulations! \nA big world... with no boundary!",
			L"哇哦，恭喜你！\n\n一个没有边际的广阔世界" },
			{ L"Wait, what the...?",
			L"等等，那是……？" },
			{ L"Is that... an enemy?",
			L"那是一个……敌人吗？" },
			{ L"We are too small to fight him...",
			L"我们太过渺小完全无法对抗……" },
			{ L"And nowhere to run...",
			L"而且无处可逃……" }
		},
		//stage 5 + 3
		{
			{ L"What the hell have you done!",
			L"你都干了些什么！" },
			{ L"Your death attracts the enemies outside.",
			L"你的死亡吸引了墙外的敌人" },
			{ L"They break the wall and find you.",
			L"他们打破了墙壁发现了你" },
			{ L"And your friend is missing...",
			L"而你的朋友也下落不明……" },
			{ L"Stage 5 - Fight to Live",
			L"第5关 - 战斗与生存" },
			{ L"I resurrected you this time, \nnever again!",
			L"这次我复活了你，但不会有下次了" },
			{ L"Now, become stronger, \nthen destory and eat them.",
			L"现在，变得更强，击溃并吞噬敌人" }
		},
		//stage 6 + 3
		{
			{ L"Stage 6 - Self",
			L"第6关 - 一人" },
			{ L"You are strong enough now.",
			L"你已经足够强大了" },
			{ L"Even the strongest enemy \nis just a piece of cake to you.",
			L"即使这里最强的敌人对你也不过是小菜一碟" },
			{ L"Just fullfill the world \nand reach the outer world.",
			L"总之像最初一样占据整个世界吧" },
		},
		//stage 7 + 3
		{
			{ L"Wow, congratulations! \nA big world... with no boundary!",
			L"哇哦，恭喜你！\n\n一个没有边际的广阔世界" },
			{ L"Wait, what the...?",
			L"等等，那是……？" },
			{ L"Is that... an enemy?",
			L"那是一个……敌人吗？" },
			{ L"We are too small to fight him...",
			L"我们太过渺小完全无法对抗……" },
			{ L"And nowhere to run...",
			L"而且无处可逃……" },
			{ L"Oh god... Another one?",
			L"天哪……还有一个？" },
			{ L"Bad luck. Maybe time to say goodbye.",
			L"运气真差。是时候说再见了" },
			{ L"...?",
			L"……" },
			{ L"He is... trying to protect you...?",
			L"他似乎在……试图保护你？" },
			{ L"Chance! Run !Now!",
			L"好机会，快逃！" }
		},
	};

	std::vector<VECLPCWSTR> m_vecEnd =
	{
		//stage 1
		{ L"Ending 0 - Stay in the Small World",
		L"结局0 - 留在渺小的世界" },
		//stage 2
		{ L"Now break the wall. \nDon't be afraid, just go straight towards it.",
		L"现在，打破墙壁\n\n不必害怕，只要冲向它就行了" },
		//stage 3
		{ L"Now do that again,\nyou will be able to get out.",
		L"现在，再次打破墙壁到达外侧" },
		//stage 4
		{ L"Now, I've controlled him. \nJust watch him die.",
		L"现在，我控制了他，静静看着他走向死亡吧" },
		//stage 5
		{ L"You can get to the outer world now.",
		L"你可以前往更外侧的世界了" },
		//stage 6
		{ L"",
		L"" },
		//stage 7
		{ L"Ending 1 - Despair",
		L"结局1 - 绝望" },
		//stage 5 + 3
		{ L"You can get to the outer world now.",
		L"你可以前往更外侧的世界了" },
		//stage 6 + 3
		{ L"",
		L"" },
		//stage 7 + 3
		{ L"Ending 2 - Protected",
		L"结局2 - 保护" }
	};
};

#endif // !SCENESNAKE_H
