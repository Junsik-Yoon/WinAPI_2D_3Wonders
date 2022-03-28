#include "pch.h"
#include "CCore.h"
#include "CTexture.h"
#include "CScene.h"

CCore::CCore()
{
	m_debugOn = false;
}
CCore::~CCore()
{

}

void CCore::update()
{
	if (KEYDOWN(VK_LSHIFT))
	{
		m_debugOn= !m_debugOn;
	}

	CEventManager::getInst()->update();
	CTimeManager::getInst()->update();
	CKeyManager::getInst()->update();
	CSoundManager::getInst()->update();
	CSceneManager::getInst()->update();
	CCollisionManager::getInst()->update();
	CCameraManager::getInst()->update();
	CUIManager::getInst()->update();
	CGameManager::getInst()->update();
}

void CCore::render()
{
	CRenderManager::getInst()->GetRenderTarget()->BeginDraw();
	CRenderManager::getInst()->RenderFillRectangle(-1, -1, WINSIZEX + 1, WINSIZEY + 1, RGB(255, 255, 255));
	CSceneManager::getInst()->render();
	CCameraManager::getInst()->render();
	render_information();

	CRenderManager::getInst()->GetRenderTarget()->EndDraw();
}

void CCore::render_information()
{

	if (CCore::getInst()->DebugMode())
	{
		CD2DImage* pImg = CResourceManager::getInst()->LoadD2DImage(L"BackInfo", L"texture\\BackInfo.png");

		////////////////////////
		wstring curScene = {};
		////////////////////////
		CScene* pCurScene = CSceneManager::getInst()->GetCurScene();
		
		curScene = pCurScene->GetName();

		CRenderManager::getInst()->RenderImage(
			pImg,
			WINSIZEX - 105.f,
			0,
			WINSIZEX,
			70.f,
			0.1f);

		CRenderManager::getInst()->RenderText(
			L" FPS :   " + std::to_wstring(CTimeManager::getInst()->GetFPS()) + L"\n" +
			L" "+ curScene + L"\n",
			WINSIZEX - 100.f,
			0,
			WINSIZEX,
			70.f,
			13.f,
			RGB(0, 0, 0));
	}
}

void CCore::init()
{
	CGameManager::getInst()->init();
	CPathManager::getInst()->init();
	CTimeManager::getInst()->init();
	CKeyManager::getInst()->init();
	CSoundManager::getInst()->init();
	CRenderManager::getInst()->init();
	CCameraManager::getInst()->init();
	CSceneManager::getInst()->init();
	CCollisionManager::getInst()->init();
}




