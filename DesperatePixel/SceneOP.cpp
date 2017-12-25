#include "DXColor.h"
#include "DXInput.h"
#include "SceneOP.h"

SceneOP::SceneOP()
{

}

SceneOP::~SceneOP()
{

}

void SceneOP::Init(int iInitVal)
{
	GetScrn().Set(false, WHITE);
	GetScrn().Refresh(IMM, NULL);

	SetRect(&m_rcLogo, 88, 69, 149, 83);
	m_iCounter = 0;
	m_fTimer = 0.0f;

	m_bKeyUp = false;
	m_bSceneOver = false;

	int xLB, yLB;

	//S

	xLB = m_rcLogo.left + 2;
	yLB = m_rcLogo.bottom - 3;

	m_vecLogo.push_back({ xLB + 0,yLB - 1 });
	m_vecLogo.push_back({ xLB + 0,yLB - 2 });
	m_vecLogo.push_back({ xLB + 0,yLB - 6 });
	m_vecLogo.push_back({ xLB + 0,yLB - 7 });
	m_vecLogo.push_back({ xLB + 0,yLB - 8 });
	m_vecLogo.push_back({ xLB + 1,yLB - 0 });
	m_vecLogo.push_back({ xLB + 1,yLB - 1 });
	m_vecLogo.push_back({ xLB + 1,yLB - 2 });
	m_vecLogo.push_back({ xLB + 1,yLB - 5 });
	m_vecLogo.push_back({ xLB + 1,yLB - 6 });
	m_vecLogo.push_back({ xLB + 1,yLB - 7 });
	m_vecLogo.push_back({ xLB + 1,yLB - 8 });
	m_vecLogo.push_back({ xLB + 1,yLB - 9 });
	m_vecLogo.push_back({ xLB + 2,yLB - 0 });
	m_vecLogo.push_back({ xLB + 2,yLB - 5 });
	m_vecLogo.push_back({ xLB + 2,yLB - 9 });
	m_vecLogo.push_back({ xLB + 3,yLB - 0 });
	m_vecLogo.push_back({ xLB + 3,yLB - 4 });
	m_vecLogo.push_back({ xLB + 3,yLB - 5 });
	m_vecLogo.push_back({ xLB + 3,yLB - 9 });
	m_vecLogo.push_back({ xLB + 4,yLB - 0 });
	m_vecLogo.push_back({ xLB + 4,yLB - 4 });
	m_vecLogo.push_back({ xLB + 4,yLB - 9 });
	m_vecLogo.push_back({ xLB + 5,yLB - 0 });
	m_vecLogo.push_back({ xLB + 5,yLB - 1 });
	m_vecLogo.push_back({ xLB + 5,yLB - 2 });
	m_vecLogo.push_back({ xLB + 5,yLB - 3 });
	m_vecLogo.push_back({ xLB + 5,yLB - 4 });
	m_vecLogo.push_back({ xLB + 5,yLB - 7 });
	m_vecLogo.push_back({ xLB + 5,yLB - 8 });
	m_vecLogo.push_back({ xLB + 5,yLB - 9 });
	m_vecLogo.push_back({ xLB + 6,yLB - 1 });
	m_vecLogo.push_back({ xLB + 6,yLB - 2 });
	m_vecLogo.push_back({ xLB + 6,yLB - 3 });
	m_vecLogo.push_back({ xLB + 6,yLB - 7 });
	m_vecLogo.push_back({ xLB + 6,yLB - 8 });

	//t

	xLB = m_rcLogo.left + 11;

	m_vecLogo.push_back({ xLB + 0,yLB - 5 });
	m_vecLogo.push_back({ xLB + 1,yLB - 1 });
	m_vecLogo.push_back({ xLB + 1,yLB - 2 });
	m_vecLogo.push_back({ xLB + 1,yLB - 3 });
	m_vecLogo.push_back({ xLB + 1,yLB - 4 });
	m_vecLogo.push_back({ xLB + 1,yLB - 5 });
	m_vecLogo.push_back({ xLB + 1,yLB - 6 });
	m_vecLogo.push_back({ xLB + 1,yLB - 7 });
	m_vecLogo.push_back({ xLB + 2,yLB - 0 });
	m_vecLogo.push_back({ xLB + 2,yLB - 1 });
	m_vecLogo.push_back({ xLB + 2,yLB - 2 });
	m_vecLogo.push_back({ xLB + 2,yLB - 3 });
	m_vecLogo.push_back({ xLB + 2,yLB - 4 });
	m_vecLogo.push_back({ xLB + 2,yLB - 5 });
	m_vecLogo.push_back({ xLB + 2,yLB - 6 });
	m_vecLogo.push_back({ xLB + 2,yLB - 7 });
	m_vecLogo.push_back({ xLB + 3,yLB - 0 });
	m_vecLogo.push_back({ xLB + 3,yLB - 5 });
	m_vecLogo.push_back({ xLB + 4,yLB - 0 });
	m_vecLogo.push_back({ xLB + 4,yLB - 5 });

	//i

	xLB = m_rcLogo.left + 18;

	m_vecLogo.push_back({ xLB + 0,yLB - 0 });
	m_vecLogo.push_back({ xLB + 0,yLB - 1 });
	m_vecLogo.push_back({ xLB + 0,yLB - 2 });
	m_vecLogo.push_back({ xLB + 0,yLB - 3 });
	m_vecLogo.push_back({ xLB + 0,yLB - 4 });
	m_vecLogo.push_back({ xLB + 0,yLB - 5 });
	m_vecLogo.push_back({ xLB + 0,yLB - 7 });
	m_vecLogo.push_back({ xLB + 0,yLB - 8 });
	m_vecLogo.push_back({ xLB + 1,yLB - 0 });
	m_vecLogo.push_back({ xLB + 1,yLB - 1 });
	m_vecLogo.push_back({ xLB + 1,yLB - 2 });
	m_vecLogo.push_back({ xLB + 1,yLB - 3 });
	m_vecLogo.push_back({ xLB + 1,yLB - 4 });
	m_vecLogo.push_back({ xLB + 1,yLB - 5 });
	m_vecLogo.push_back({ xLB + 1,yLB - 7 });
	m_vecLogo.push_back({ xLB + 1,yLB - 8 });

	//l

	xLB = m_rcLogo.left + 23;

	m_vecLogo.push_back({ xLB + 0,yLB - 0 });
	m_vecLogo.push_back({ xLB + 0,yLB - 1 });
	m_vecLogo.push_back({ xLB + 0,yLB - 2 });
	m_vecLogo.push_back({ xLB + 0,yLB - 3 });
	m_vecLogo.push_back({ xLB + 0,yLB - 4 });
	m_vecLogo.push_back({ xLB + 0,yLB - 5 });
	m_vecLogo.push_back({ xLB + 0,yLB - 6 });
	m_vecLogo.push_back({ xLB + 0,yLB - 7 });
	m_vecLogo.push_back({ xLB + 0,yLB - 8 });
	m_vecLogo.push_back({ xLB + 0,yLB - 9 });
	m_vecLogo.push_back({ xLB + 1,yLB - 0 });
	m_vecLogo.push_back({ xLB + 1,yLB - 1 });
	m_vecLogo.push_back({ xLB + 1,yLB - 2 });
	m_vecLogo.push_back({ xLB + 1,yLB - 3 });
	m_vecLogo.push_back({ xLB + 1,yLB - 4 });
	m_vecLogo.push_back({ xLB + 1,yLB - 5 });
	m_vecLogo.push_back({ xLB + 1,yLB - 6 });
	m_vecLogo.push_back({ xLB + 1,yLB - 7 });
	m_vecLogo.push_back({ xLB + 1,yLB - 8 });
	m_vecLogo.push_back({ xLB + 1,yLB - 9 });

	//l

	xLB = m_rcLogo.left + 28;

	m_vecLogo.push_back({ xLB + 0,yLB - 0 });
	m_vecLogo.push_back({ xLB + 0,yLB - 1 });
	m_vecLogo.push_back({ xLB + 0,yLB - 2 });
	m_vecLogo.push_back({ xLB + 0,yLB - 3 });
	m_vecLogo.push_back({ xLB + 0,yLB - 4 });
	m_vecLogo.push_back({ xLB + 0,yLB - 5 });
	m_vecLogo.push_back({ xLB + 0,yLB - 6 });
	m_vecLogo.push_back({ xLB + 0,yLB - 7 });
	m_vecLogo.push_back({ xLB + 0,yLB - 8 });
	m_vecLogo.push_back({ xLB + 0,yLB - 9 });
	m_vecLogo.push_back({ xLB + 1,yLB - 0 });
	m_vecLogo.push_back({ xLB + 1,yLB - 1 });
	m_vecLogo.push_back({ xLB + 1,yLB - 2 });
	m_vecLogo.push_back({ xLB + 1,yLB - 3 });
	m_vecLogo.push_back({ xLB + 1,yLB - 4 });
	m_vecLogo.push_back({ xLB + 1,yLB - 5 });
	m_vecLogo.push_back({ xLB + 1,yLB - 6 });
	m_vecLogo.push_back({ xLB + 1,yLB - 7 });
	m_vecLogo.push_back({ xLB + 1,yLB - 8 });
	m_vecLogo.push_back({ xLB + 1,yLB - 9 });

	//B

	xLB = m_rcLogo.left + 36;

	m_vecLogo.push_back({ xLB + 0,yLB - 0 });
	m_vecLogo.push_back({ xLB + 0,yLB - 1 });
	m_vecLogo.push_back({ xLB + 0,yLB - 2 });
	m_vecLogo.push_back({ xLB + 0,yLB - 3 });
	m_vecLogo.push_back({ xLB + 0,yLB - 4 });
	m_vecLogo.push_back({ xLB + 0,yLB - 5 });
	m_vecLogo.push_back({ xLB + 0,yLB - 6 });
	m_vecLogo.push_back({ xLB + 0,yLB - 7 });
	m_vecLogo.push_back({ xLB + 0,yLB - 8 });
	m_vecLogo.push_back({ xLB + 0,yLB - 9 });
	m_vecLogo.push_back({ xLB + 1,yLB - 0 });
	m_vecLogo.push_back({ xLB + 1,yLB - 1 });
	m_vecLogo.push_back({ xLB + 1,yLB - 2 });
	m_vecLogo.push_back({ xLB + 1,yLB - 3 });
	m_vecLogo.push_back({ xLB + 1,yLB - 4 });
	m_vecLogo.push_back({ xLB + 1,yLB - 5 });
	m_vecLogo.push_back({ xLB + 1,yLB - 6 });
	m_vecLogo.push_back({ xLB + 1,yLB - 7 });
	m_vecLogo.push_back({ xLB + 1,yLB - 8 });
	m_vecLogo.push_back({ xLB + 1,yLB - 9 });
	m_vecLogo.push_back({ xLB + 2,yLB - 0 });
	m_vecLogo.push_back({ xLB + 2,yLB - 5 });
	m_vecLogo.push_back({ xLB + 2,yLB - 9 });
	m_vecLogo.push_back({ xLB + 3,yLB - 0 });
	m_vecLogo.push_back({ xLB + 3,yLB - 5 });
	m_vecLogo.push_back({ xLB + 3,yLB - 9 });
	m_vecLogo.push_back({ xLB + 4,yLB - 0 });
	m_vecLogo.push_back({ xLB + 4,yLB - 5 });
	m_vecLogo.push_back({ xLB + 4,yLB - 9 });
	m_vecLogo.push_back({ xLB + 5,yLB - 0 });
	m_vecLogo.push_back({ xLB + 5,yLB - 1 });
	m_vecLogo.push_back({ xLB + 5,yLB - 2 });
	m_vecLogo.push_back({ xLB + 5,yLB - 3 });
	m_vecLogo.push_back({ xLB + 5,yLB - 4 });
	m_vecLogo.push_back({ xLB + 5,yLB - 5 });
	m_vecLogo.push_back({ xLB + 5,yLB - 6 });
	m_vecLogo.push_back({ xLB + 5,yLB - 7 });
	m_vecLogo.push_back({ xLB + 5,yLB - 8 });
	m_vecLogo.push_back({ xLB + 5,yLB - 9 });
	m_vecLogo.push_back({ xLB + 6,yLB - 1 });
	m_vecLogo.push_back({ xLB + 6,yLB - 2 });
	m_vecLogo.push_back({ xLB + 6,yLB - 3 });
	m_vecLogo.push_back({ xLB + 6,yLB - 4 });
	m_vecLogo.push_back({ xLB + 6,yLB - 6 });
	m_vecLogo.push_back({ xLB + 6,yLB - 7 });
	m_vecLogo.push_back({ xLB + 6,yLB - 8 });

	//o

	xLB = m_rcLogo.left + 45;

	m_vecLogo.push_back({ xLB + 0,yLB - 1 });
	m_vecLogo.push_back({ xLB + 0,yLB - 2 });
	m_vecLogo.push_back({ xLB + 0,yLB - 3 });
	m_vecLogo.push_back({ xLB + 0,yLB - 4 });
	m_vecLogo.push_back({ xLB + 1,yLB - 0 });
	m_vecLogo.push_back({ xLB + 1,yLB - 1 });
	m_vecLogo.push_back({ xLB + 1,yLB - 2 });
	m_vecLogo.push_back({ xLB + 1,yLB - 3 });
	m_vecLogo.push_back({ xLB + 1,yLB - 4 });
	m_vecLogo.push_back({ xLB + 1,yLB - 5 });
	m_vecLogo.push_back({ xLB + 2,yLB - 0 });
	m_vecLogo.push_back({ xLB + 2,yLB - 5 });
	m_vecLogo.push_back({ xLB + 3,yLB - 0 });
	m_vecLogo.push_back({ xLB + 3,yLB - 5 });
	m_vecLogo.push_back({ xLB + 4,yLB - 0 });
	m_vecLogo.push_back({ xLB + 4,yLB - 1 });
	m_vecLogo.push_back({ xLB + 4,yLB - 2 });
	m_vecLogo.push_back({ xLB + 4,yLB - 3 });
	m_vecLogo.push_back({ xLB + 4,yLB - 4 });
	m_vecLogo.push_back({ xLB + 4,yLB - 5 });
	m_vecLogo.push_back({ xLB + 5,yLB - 1 });
	m_vecLogo.push_back({ xLB + 5,yLB - 2 });
	m_vecLogo.push_back({ xLB + 5,yLB - 3 });
	m_vecLogo.push_back({ xLB + 5,yLB - 4 });

	//x

	xLB = m_rcLogo.left + 53;

	m_vecLogo.push_back({ xLB + 0,yLB - 0 });
	m_vecLogo.push_back({ xLB + 0,yLB - 1 });
	m_vecLogo.push_back({ xLB + 0,yLB - 4 });
	m_vecLogo.push_back({ xLB + 0,yLB - 5 });
	m_vecLogo.push_back({ xLB + 1,yLB - 0 });
	m_vecLogo.push_back({ xLB + 1,yLB - 1 });
	m_vecLogo.push_back({ xLB + 1,yLB - 4 });
	m_vecLogo.push_back({ xLB + 1,yLB - 5 });
	m_vecLogo.push_back({ xLB + 2,yLB - 1 });
	m_vecLogo.push_back({ xLB + 2,yLB - 2 });
	m_vecLogo.push_back({ xLB + 2,yLB - 3 });
	m_vecLogo.push_back({ xLB + 2,yLB - 4 });
	m_vecLogo.push_back({ xLB + 3,yLB - 1 });
	m_vecLogo.push_back({ xLB + 3,yLB - 2 });
	m_vecLogo.push_back({ xLB + 3,yLB - 3 });
	m_vecLogo.push_back({ xLB + 3,yLB - 4 });
	m_vecLogo.push_back({ xLB + 4,yLB - 0 });
	m_vecLogo.push_back({ xLB + 4,yLB - 1 });
	m_vecLogo.push_back({ xLB + 4,yLB - 4 });
	m_vecLogo.push_back({ xLB + 4,yLB - 5 });
	m_vecLogo.push_back({ xLB + 5,yLB - 0 });
	m_vecLogo.push_back({ xLB + 5,yLB - 1 });
	m_vecLogo.push_back({ xLB + 5,yLB - 4 });
	m_vecLogo.push_back({ xLB + 5,yLB - 5 });
}

void SceneOP::Update(double fTime, float fElapsedTime)
{
	GetInput();

	if (IsKeyDown())
	{
		if (m_bKeyUp)
		{
			if (m_iCounter < 5)
				m_iCounter = 5;
			if (m_fTimer < 2500.0f)
				m_fTimer = 2500.0f;
		}
		m_bKeyUp = false;
	}
	else
		m_bKeyUp = true;

	if (GetScrn().IsRefOver())
	{
		switch (m_iCounter)
		{
		case 0:
			m_fTimer += fElapsedTime;
			if (m_fTimer >= 1000.0f)
			{
				m_iCounter++;
				m_fTimer = 0.0f;
			}
			break;

		case 5:
			GetScrn().Set(true, WHITE, &m_rcLogo);
			GetScrn().Set(false, WHITE, &m_vecLogo);
			GetScrn().Refresh(BRUSH, &m_rcLogo);
			m_iCounter++;
			break;

		case 6:
			m_fTimer += fElapsedTime;

			if (m_fTimer >= 2500.0f)
			{
				GetScrn().Set(false, WHITE, &m_rcLogo);
				GetScrn().Refresh(BRUSH, &m_rcLogo);
				GetScrn().SetRefPeriod(5.0f);
				m_iCounter++;
			}
			break;

		case 7:
			m_bSceneOver = true;
			break;

		default:
			GetScrn().Set(true, GRAY((4 + 3 * m_iCounter)), &m_vecLogo);			
			GetScrn().Refresh(BRUSH, &m_rcLogo);
			GetScrn().SetRefPeriod(5.0f);
			m_iCounter++;
			break;
		}
	}
	else
	{
		GetScrn().Update(fTime, fElapsedTime);
	}

	if (m_bSceneOver)
		SetScene(MENU);
}

void SceneOP::Render(double fTime, float fElapsedTime)
{
	GetScrn().Render(fTime, fElapsedTime);
}