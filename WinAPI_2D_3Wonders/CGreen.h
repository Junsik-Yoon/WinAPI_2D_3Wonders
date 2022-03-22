#pragma once
#include "CGameObject.h"

class CD2DImage;

enum class eState_Green
{
	IDLE,
	TRACE,
	SHOOT,
	SIZE,
};

class CGreen : public CGameObject
{
private:
	float m_shootFire;//½Ã°£
	bool isRight;
	eState_Green m_state;
	CD2DImage* m_pImg;
	UINT m_floor;
	UINT m_wall;
	float m_velocity;
	float m_gravity;
public:
	void ShootFire();

public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	virtual void update();
	virtual void render();
	void render_information();

public:
	CGreen();
	~CGreen();
	CGreen* Clone() { return new CGreen(*this); }
};