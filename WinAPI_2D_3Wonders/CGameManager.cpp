#include "pch.h"
#include "CGameManager.h"

CGameManager::CGameManager()
{
	g_isRendering = true;
	g_pPlayer = nullptr;
	g_gameScore = 0;
	g_playerLife = 0;
	g_heartsHave = 0;
}
CGameManager::~CGameManager()
{

}

void CGameManager::update()
{
}

void CGameManager::init()
{
}
