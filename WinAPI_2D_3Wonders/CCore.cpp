#include "pch.h"
#include "CCore.h"
#include "CTexture.h"

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
}

void CCore::render()
{
	CRenderManager::getInst()->GetRenderTarget()->BeginDraw();
	CRenderManager::getInst()->RenderFillRectangle(-1, -1, WINSIZEX + 1, WINSIZEY + 1, RGB(255, 255, 255));
	CSceneManager::getInst()->render();
	CCameraManager::getInst()->render();

	//fps
	WCHAR strFPS[6];
	swprintf_s(strFPS, L"%d", CTimeManager::getInst()->GetFPS());
	CRenderManager::getInst()->RenderText(strFPS, WINSIZEX - 50, 10, WINSIZEX, 50, 12, RGB(0, 0, 0));

	CRenderManager::getInst()->GetRenderTarget()->EndDraw();

}

void CCore::init()
{

	CPathManager::getInst()->init();
	CTimeManager::getInst()->init();
	CKeyManager::getInst()->init();
	CSoundManager::getInst()->init();
	CRenderManager::getInst()->init();
	CCameraManager::getInst()->init();
	CSceneManager::getInst()->init();
	CCollisionManager::getInst()->init();


}




