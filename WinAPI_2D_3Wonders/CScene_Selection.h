#pragma once
#include "CScene.h"
class CScene_Selection :public CScene
{	
private:

public:
	virtual void update();
	virtual void Enter();
	virtual void Exit();
public:
	CScene_Selection();
	~CScene_Selection();

};
