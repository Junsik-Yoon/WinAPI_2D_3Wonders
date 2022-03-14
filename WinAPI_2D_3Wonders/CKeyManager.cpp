#include "pch.h"
#include "CKeyManager.h"

CKeyManager::CKeyManager()
{
	for (int key = 0; key < VKEY_SIZE; ++key)
	{
		m_arrPrevKey[key] = false;
		m_arrCurKey[key] = false;
	}
}
CKeyManager::~CKeyManager()
{
}

void CKeyManager::update()
{
	HWND curWnd = GetFocus();
	if (hWnd != curWnd)
	{
		for (int key = 0; key < VKEY_SIZE; ++key)
		{
			m_arrPrevKey[key] = m_arrCurKey[key];
			m_arrCurKey[key] = false;
		}
	}

	for (int key = 0; key < VKEY_SIZE; ++key)
	{
		m_arrPrevKey[key] = m_arrCurKey[key];
		if (GetAsyncKeyState(key) & 0x8000)
		{
			m_arrCurKey[key] = true;
		}
		else
		{
			m_arrCurKey[key] = false;
		}
	}

	// Mouse 좌표 계산
	POINT ptPos = {};
	// GetCursorPos() 윈도우에서 모니터 좌상단 기준 마우스의 좌표를 반환
	GetCursorPos(&ptPos);
	// 모니터 좌상단 기준 마우스 좌표를 게임 윈도우 기준 마우스 위치로 계산
	ScreenToClient(hWnd, &ptPos);

	m_fptCurMousePos = Vec2((float)ptPos.x, (float)ptPos.y);
}

void CKeyManager::init()
{
}

