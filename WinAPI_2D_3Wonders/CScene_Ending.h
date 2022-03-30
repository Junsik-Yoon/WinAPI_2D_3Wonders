#pragma once
#include "CScene.h"


class CScene_Ending : public CScene
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
	CScene_Ending();
	~CScene_Ending();


};

