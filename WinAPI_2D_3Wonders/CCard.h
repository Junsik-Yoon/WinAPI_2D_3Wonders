#pragma once
#include "CItem.h"

class CD2DImage;

class CCard :public CItem
{
private:
	CD2DImage* m_pImg;
	int m_cardValue;
	float m_dropDir;
	float m_gravity;
public:
	void SetDropDir(float _dir) { m_dropDir = _dir; }
public:
	virtual void update();
	virtual void render();
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	CCard();
	CCard(int cardValue);
	~CCard();
};

