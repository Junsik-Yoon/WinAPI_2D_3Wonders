#pragma once
#include "CGameObject.h"

enum class eState_FChest
{
	IDLE,
	FLY,
	DESTROYED,
	FLYAWAY,

	SIZE,
};
enum class eDirection
{
	UP = 1,
	DOWN,
	LEFT,
	RIGHT
};
class CD2DImage;
class CItem;

class CFlyingChest :public CGameObject
{
private:
	eState_FChest m_state;
	CD2DImage* m_pImg;
	vector<CItem*> itemsContain;
	float m_appearTimer;
	float m_effectTimer;
	float  m_width = 2500.f;
	float  m_height = 50.f;
	float  m_fSpd = 1.f;

	Vec2 m_center;
	float m_fTheta;

public:
	void Put_Item();
	void Set_Item(CItem* _pObj) { itemsContain.push_back(_pObj); }
	void TwinkleEffect();
public:
	virtual void update();
	virtual void render();
	void information_render();
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);

public:
	CFlyingChest();
	~CFlyingChest();
	CFlyingChest* Clone() { return new CFlyingChest(*this); }

};

