#pragma once
#include "CScene.h"
class CScene_Stage1 : public CScene
{
private:
	float m_velocity = 500;
public:
	CScene_Stage1();
	~CScene_Stage1();
	virtual void update();
	virtual void Enter();
	virtual void Exit();
};
