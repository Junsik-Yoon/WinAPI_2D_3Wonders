#pragma once
#include "CGameObject.h"

class CD2DImage;

enum class eState_Bug
{
	NONE,
	FLY,
	HOP,
	FLYAWAY,
	SIZE,
};

class CBug :  public CGameObject
{
	friend class CHalfMoon;
private:
	CD2DImage* m_pImg;
	eState_Bug m_state;
	UINT m_floor;
	UINT m_wall;
	float m_velocity;
	float m_gravity;
	bool isHeadedRight;
public:
	void Fly();
	void Hop();
	void FlyAway();

public:
	void SetState(eState_Bug _state) { m_state = _state; }
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	virtual void update();
	virtual void render();
	void render_information();
public:
	CBug();
	~CBug();
	CBug* Clone() { return new CBug(*this); }
};

