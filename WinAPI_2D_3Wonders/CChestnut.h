#pragma once
#include "CGameObject.h"

class CD2DImage;

class CChestnut : public CGameObject
{

public:
	virtual void update();
	virtual void render();
public:
	void OnCollisionEnter(CCollider* pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	CChestnut();
	~CChestnut();
	virtual CChestnut* Clone() { return new CChestnut(*this); }
};

