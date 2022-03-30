#include "pch.h"
#include "CScene_Opening.h"

#include <Vfw.h>
#pragma comment(lib, "Vfw32.lib")

CScene_Opening::CScene_Opening()
{

}

CScene_Opening::~CScene_Opening()
{

}

void CScene_Opening::Initialize()
{
	// 비디오 윈도우 핸들에 세팅 및 재생
	m_hVideo = MCIWndCreate(hWnd, NULL, MCIWNDF_NOPLAYBAR | WS_VISIBLE | WS_CHILD
		, L"content\\video\\opening.wmv");
	MoveWindow(m_hVideo, 0, 0, WINSIZEX, WINSIZEY, false);

	MCIWndPlay(m_hVideo);
}

void CScene_Opening::update()
{
	CScene::update();
	if (KEYDOWN('Z') || KEYDOWN('X') || MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo))
	{
		ChangeScn(GROUP_SCENE::STAGE1);
	}
}

void CScene_Opening::Enter()
{
	CGameManager::getInst()->SetRender(false);
	Initialize();
	//CCameraManager::getInst()->SetLookAt(Vec2(float(WINSIZEX / 2.f), float(WINSIZEY / 2.f)));
}

void CScene_Opening::Exit()
{
	MCIWndClose(m_hVideo);
	CGameManager::getInst()->SetRender(true);
	DeleteAll();
}
