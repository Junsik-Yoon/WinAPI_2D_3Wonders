#pragma once
#include "CGameObject.h"

class CD2DImage;

class CGreen : public CGameObject
{
private:
	CD2DImage* m_pImg;
	UINT m_floor;
	UINT m_wall;
	float m_velocity;
	float m_gravity;
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	virtual void update();
	virtual void render();
public:
	CGreen();
	~CGreen();
	CGreen* Clone() { return new CGreen(*this); }
};