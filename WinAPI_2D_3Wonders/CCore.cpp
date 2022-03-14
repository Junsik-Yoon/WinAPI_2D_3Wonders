#include "pch.h"
#include "CCore.h"
#include "CTexture.h"

CCore::CCore()
{
	m_hDC = 0;

	m_arrPen[0] = 0;
	m_arrBrush[0] = 0;
}
CCore::~CCore()
{
	ReleaseDC(hWnd, m_hDC);

	for (int i = 0; i < (int)TYPE_PEN::SIZE; i++)
	{
		DeleteObject(m_arrPen[i]);
	}
}

void CCore::update()
{
	CEventManager::getInst()->update();
	CTimeManager::getInst()->update();
	CKeyManager::getInst()->update();
	CSoundManager::getInst()->update();
	CSceneManager::getInst()->update();
	CCollisionManager::getInst()->update();
	CCameraManager::getInst()->update();
	CUIManager::getInst()->update();
}

void CCore::render()
{
	Rectangle(m_pMemTex->GetDC(), -1, -1, WINSIZEX + 1, WINSIZEY + 1);

	CSceneManager::getInst()->render(m_pMemTex->GetDC());
	CCameraManager::getInst()->render(m_pMemTex->GetDC());

	//fps
	WCHAR strFPS[6];
	swprintf_s(strFPS, L"%d", CTimeManager::getInst()->GetFPS());
	TextOutW(m_pMemTex->GetDC(), WINSIZEX - 50, 10, strFPS, 5);
	//SetWindowTextW(hWnd, strFPS);

	BitBlt(m_hDC, 0, 0, WINSIZEX, WINSIZEY, m_pMemTex->GetDC(), 0, 0, SRCCOPY);
}

void CCore::init()
{
	// GDI
	CreateBrushPen();
	m_hDC = GetDC(hWnd);

	CPathManager::getInst()->init();
	CTimeManager::getInst()->init();
	CKeyManager::getInst()->init();
	CSoundManager::getInst()->init();
	CSceneManager::getInst()->init();
	CCollisionManager::getInst()->init();
	CCameraManager::getInst()->init();

	m_pMemTex = CResourceManager::getInst()->CreateTexture(L"CoreTex", WINSIZEX, WINSIZEY);

}

void CCore::CreateBrushPen()
{
	// brush
	m_arrBrush[(int)TYPE_BRUSH::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	// pen
	m_arrPen[(int)TYPE_PEN::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(int)TYPE_PEN::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(int)TYPE_PEN::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}




