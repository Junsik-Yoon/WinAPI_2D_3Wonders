#pragma once
#include "CItem.h"

class CLou;
class CD2DImage;

enum class eState_OpFire
{
	NO_OWNER,
	IDLE,
	MOVE,
	ATTACK,

	SIZE,
};

class COptionFire : public CItem
{
	friend class CLou;
private:
	bool isFacedRight;
	CD2DImage* m_pImg;
	eState_OpFire m_state;
	CLou* m_owner;
public:
	virtual void update();
	virtual void render();
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	COptionFire();
	~COptionFire();
	COptionFire* Clone() { return new COptionFire(*this); }
};

