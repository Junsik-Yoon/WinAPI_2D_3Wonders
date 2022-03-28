#pragma once
#include "CGameObject.h"

class CD2DImage;

class CChestnut : public CGameObject
{
private:
	CD2DImage* m_pImg;
	bool isShaking;
	float m_gravity;
	float m_velocity;
	float m_upforce;
	int m_floor;
	Vec2 offset;
	bool onTree = false;

public:
	void Shake();
	void MoveForward();
	void SetOffset(Vec2 _offset) { offset = _offset; }
	Vec2 GetOffset() { return offset; }
	void SetOnTree(bool _isTree) { onTree = _isTree; }
public:
	virtual void update();
	virtual void render();
public:
	void OnCollisionEnter(CCollider* pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	CChestnut();
	~CChestnut();
	virtual CChestnut* Clone() { return new CChestnut(*this); }
};

