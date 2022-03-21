#pragma once
#include "CTile.h"

class CD2DImage;

class CMovingTile : public CTile
{
	friend class CGolemWood;
private:
	CD2DImage* m_pImg;
	GROUP_TILE m_eGroup;

	UINT m_state;

	int Y_axis;

	bool isRight;
	bool isEndline;

	float nextTileCounter;
public:
	void SetRight(bool _isRight) { isRight = _isRight; }
	void SetState(UINT _state) { m_state = _state; }
public:
	virtual void update();
	virtual void render();
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);

public:
	CMovingTile();
	~CMovingTile();
};

