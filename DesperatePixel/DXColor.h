#pragma once

#ifndef DXCOLOR_H
#define DXCOLOR_H

#include <d3dx9.h>

#define BLACK	D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f)
#define WHITE	D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f)

#define GRAY(x) D3DXCOLOR(0.0625f * x, 0.0625f * x, 0.0625f * x, 1.0f)

#define RED		D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f)

#define GREEN	D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f)

#define BLUE	D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f)

#define YELLOW	D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f)

//WOW

#define MAGE		D3DXCOLOR(0.41f, 0.80f, 0.94f, 1.0f)
#define MAGE_D		D3DXCOLOR(0.31f, 0.60f, 0.71f, 1.0f)

#define MONK		D3DXCOLOR(0.00f, 1.00f, 0.59f, 1.0f)
#define MONK_D		D3DXCOLOR(0.00f, 0.75f, 0.45f, 1.0f)

#define DRUID		D3DXCOLOR(1.00f, 0.49f, 0.04f, 1.0f)
#define DRUID_D		D3DXCOLOR(0.75f, 0.37f, 0.03f, 1.0f)

#define ROGUE		D3DXCOLOR(1.00f, 0.96f, 0.41f, 1.0f)
#define ROGUE_D		D3DXCOLOR(0.75f, 0.72f, 0.31f, 1.0f)

#define HUNTER		D3DXCOLOR(0.67f, 0.83f, 0.45f, 1.0f)
#define HUNTER_D	D3DXCOLOR(0.51f, 0.63f, 0.34f, 1.0f)

#define PRIEST		D3DXCOLOR(1.00f, 1.00f, 1.00f, 1.0f)
#define PRIEST_D	D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f)

#define SHAMAN		D3DXCOLOR(0.00f, 0.44f, 0.87f, 1.0f)
#define SHAMAN_D	D3DXCOLOR(0.00f, 0.33f, 0.63f, 1.0f)

#define DKNIGHT		D3DXCOLOR(0.77f, 0.12f, 0.23f, 1.0f)
#define DKNIGHT_D	D3DXCOLOR(0.58f, 0.09f, 0.18f, 1.0f)

#define DHUNTER		D3DXCOLOR(0.64f, 0.19f, 0.79f, 1.0f)
#define DHUNTER_D	D3DXCOLOR(0.48f, 0.15f, 0.60f, 1.0f)

#define PALADIN		D3DXCOLOR(0.96f, 0.55f, 0.73f, 1.0f)
#define PALADIN_D	D3DXCOLOR(0.72f, 0.42f, 0.55f, 1.0f)

#define WARLOCK		D3DXCOLOR(0.58f, 0.51f, 0.79f, 1.0f)
#define WARLOCK_D	D3DXCOLOR(0.44f, 0.39f, 0.60f, 1.0f)

#define WARRIOR		D3DXCOLOR(0.78f, 0.61f, 0.43f, 1.0f)
#define WARRIOR_D	D3DXCOLOR(0.59f, 0.46f, 0.33f, 1.0f)

#endif // !DXCOLOR_H
