#pragma once
#include "CItem.h"

enum class eType
{
	HYPER = 1,
	BOUND,
	TAIL,
};

class CD2DImage;
class CMissileItem :public CItem
{
private:
	CD2DImage* m_pImg;
	eType m_missileType;
public:
	void SetMissileType(eType _missileType);
	void SetRandomMissile();
public:
	virtual void update();
	virtual void render();
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	CMissileItem();
	~CMissileItem();
};

