#pragma once
#include "CScene.h"
class CScene_Intro :
    public CScene
{
private:
	HWND m_hVideo;
	bool isVideoFinished;
public:
	void Initialize();
public:
	virtual void update();
	virtual void Enter();
	virtual void Exit();
public:
	CScene_Intro();
	~CScene_Intro();
};
