#pragma once
#include "CGameObject.h"

enum class eState_Chest
{
	INVISIBLE,
	IDLE,
	HIT,
	DESTROYED,

	SIZE,
};

class CD2DImage;
class CItem; 

class CChest : public CGameObject
{
private:
	int prevHP;
	eState_Chest m_state;
	CD2DImage* m_pImg;
	float m_vanishTimer;
	bool isVisible;
	CItem* itemContains;

public:
	void Put_Item();
	void Set_Item(CItem* _pObj) { itemContains = _pObj; }
public:
	void SetVisibility(bool _visible) { isVisible = _visible; }
public:
	virtual void update();
	virtual void render();
	void information_render();
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);

public:
	CChest();
	~CChest();
	CChest* Clone() { return new CChest(*this); }
};

