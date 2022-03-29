#pragma once
#include "CGameObject.h"

class CD2DImage;

class COwl :public CGameObject
{
private:
	CD2DImage* m_pImg;
	float  deadCounter;
	bool isDead;

public:
	void SetDead(bool _isDead) { isDead = _isDead; }
	void FollowPlayer();
	void AttackEntireWindow();
	void AttackEffect();

public:
	virtual void update();
	virtual void render();

public:
	COwl();
	COwl(Vec2 _pos);
	~COwl();
	COwl* Clone() { return new COwl(*this); }
};

