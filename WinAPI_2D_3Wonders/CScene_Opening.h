#pragma once
#include "CScene.h"
class CScene_Opening :
    public CScene
{
private:
	HWND m_hVideo;
public:
	void Initialize();
public:
	virtual void update();
	virtual void Enter();
	virtual void Exit();
public:
	CScene_Opening();
	~CScene_Opening();
};

