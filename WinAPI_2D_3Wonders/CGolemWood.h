#pragma once
#include "CGameObject.h"

class CMovingTile;
class CD2DImage;
class CChestnut;

enum class eState_GW
{
	IDLE,
	PREPARE,
	TILEMOVE,
	ATTACK,
	DEAD,

	SIZE,
};

class CGolemWood : public CGameObject
{
private:
	CD2DImage* m_pImg;

	eState_GW m_state;

	vector<CMovingTile*> pMovingTiles;
	vector<CChestnut*> pChestnuts;

	float m_tileSpeed;
	int m_floor;
	float m_gravity;

	bool positionAdjust = false;

	double m_dMoveTimer=0;
	float m_chestnutTimer = 0.f;

	float m_bossTimer;
	float m_shootTimer;
	bool canShoot = false;

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
	void SetMovingTiles(CMovingTile* pMovingTile) { pMovingTiles.push_back(pMovingTile); }
public:
	CGolemWood();
	~CGolemWood();
	CGolemWood* Clone() { return new CGolemWood(*this); }

};

