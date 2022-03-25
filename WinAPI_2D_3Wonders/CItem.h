#pragma once
#include "CGameObject.h"
class CItem : public CGameObject
{

public:
	virtual void update();
	virtual void render();
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	CItem();
	~CItem();
	CItem* Clone() { return new CItem(*this); }
};

