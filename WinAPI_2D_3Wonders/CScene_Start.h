#pragma once
#include "CScene.h"
class CScene_Start : public CScene
{
private:
	float m_velocity = 200;
public:
	CScene_Start();
	~CScene_Start();
	virtual void update();
	virtual void Enter();
	virtual void Exit();
};
