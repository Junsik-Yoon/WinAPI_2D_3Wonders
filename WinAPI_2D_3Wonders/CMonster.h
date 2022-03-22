#pragma once
#include "CGameObject.h"
class CMonster :public CGameObject
{



public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	virtual void update();
	virtual void render();
	void render_information();
public:
	CMonster();
	~CMonster();
	CMonster* Clone() { return new CMonster(*this); }
};

