#pragma once
#include "CGameObject.h"

class CD2DImage;

enum class eState_Bug
{
	NONE,
	BORN,
	FLY,
	HOP,
	IDLE,
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
	float m_lifeCycle;
	float m_velocity;
	float m_gravity;
	float m_upforce;
	bool isFacingRight;
	bool isGoingRight;
	float m_highJump;
	bool isJump;
public:
	void Hop(float _velocity);

public:
	void SetState(eState_Bug _state) { m_state = _state; }
	void SetGoRight(bool _isHeadedRight) { isGoingRight = _isHeadedRight; }
	void SetFaceRight(bool _isFacingRight) { isFacingRight = _isFacingRight; }
	bool GetGoRight() { return isGoingRight; }
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	virtual void update();
	virtual void render();
	void update_move();
	void update_animation();
	void render_information();
public:
	CBug();
	~CBug();
	CBug* Clone() { return new CBug(*this); }
};

