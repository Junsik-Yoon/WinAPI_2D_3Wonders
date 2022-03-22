#pragma once
#include "CGameObject.h"

class CMovingTile;
class CD2DImage;

class CGolemWood : public CGameObject
{
private:
	CD2DImage* m_pImg;

	float m_tileSpeed;
	int m_floor;
	float m_gravity;

	float m_missileCounter;
	float m_stopCounter;

public:
	vector<CMovingTile*> pMovingTiles;//테스트 후 private
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	virtual void update();
	virtual void render();
	void render_information();

public:
	void CreateMissile();
	void MoveTiles();
	void DropChestnut();
public:
	CGolemWood();
	~CGolemWood();
	CGolemWood* Clone() { return new CGolemWood(*this); }

};

