#include "pch.h"
#include "CScene_Intro.h"

#include <Vfw.h>
#pragma comment(lib, "Vfw32.lib")

CScene_Intro::CScene_Intro()
{
	isVideoFinished = false;
}

CScene_Intro::~CScene_Intro()
{
}



void CScene_Intro::update()
{
	CScene::update();
	
	if (MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo))
	{
		ChangeScn(GROUP_SCENE::SELECT);
	}
}

void CScene_Intro::Enter()
{
	CGameManager::getInst()->SetRender(false);
	Initialize();
}

void CScene_Intro::Exit()
{
	MCIWndClose(m_hVideo);
	CGameManager::getInst()->SetRender(true);
	DeleteAll();
}


void CScene_Intro::Initialize()
{
	// ���� ������ �ڵ鿡 ���� �� ���
	m_hVideo = MCIWndCreate(hWnd, NULL, MCIWNDF_NOPLAYBAR | WS_VISIBLE | WS_CHILD
		, L"content\\video\\intro.wmv");
	MoveWindow(m_hVideo, 0, 0, WINSIZEX, WINSIZEY, false);

	MCIWndPlay(m_hVideo);
}
