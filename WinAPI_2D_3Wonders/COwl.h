#pragma once
#include "CGameObject.h"

class CD2DImage;

class COwl :public CGameObject
{
private:
	CD2DImage* m_pImg;

public:
	void FollowPlayer();
	void AttackEntireWindow();

public:
	virtual void update();
	virtual void render();

public:
	COwl();
	COwl(Vec2 _pos);
	~COwl();
	COwl* Clone() { return new COwl(*this); }
};

