#pragma once
#include "CGameObject.h"
class CBossTrigger :
    public CGameObject
{
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	virtual void update();
	virtual void render();
public:
	CBossTrigger();
	~CBossTrigger();
	CBossTrigger* Clone() { return new CBossTrigger(*this); }
};

