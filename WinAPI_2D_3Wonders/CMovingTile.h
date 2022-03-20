#pragma once
#include "CTile.h"

class CD2DImage;

class CMovingTile : public CTile
{
	friend class CGolemWood;
private:
	CD2DImage* m_pImg;
	GROUP_TILE m_eGroup;

	int Y_axis;

	bool isRight;

	float nextTileCounter;

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

