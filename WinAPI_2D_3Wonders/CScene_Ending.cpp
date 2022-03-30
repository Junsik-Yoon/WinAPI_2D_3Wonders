#include "pch.h"
#include "CScene_Ending.h"


#include <Vfw.h>
#pragma comment(lib, "Vfw32.lib")

CScene_Ending::CScene_Ending()
{
}

CScene_Ending::~CScene_Ending()
{
}

void CScene_Ending::update()
{
	CScene::update();

}

void CScene_Ending::Enter()
{
	CGameManager::getInst()->SetRender(false);
	Initialize();
}

void CScene_Ending::Exit()
{
	MCIWndClose(m_hVideo);
	CGameManager::getInst()->SetRender(true);
	DeleteAll();
}


void CScene_Ending::Initialize()
{
	
	// 비디오 윈도우 핸들에 세팅 및 재생
	m_hVideo = MCIWndCreate(hWnd, NULL, MCIWNDF_NOPLAYBAR | WS_VISIBLE | WS_CHILD 
		, L"content\\video\\ending.wmv");
	MoveWindow(m_hVideo, 0, 0, WINSIZEX, WINSIZEY, false);

	MCIWndPlay(m_hVideo);
}





