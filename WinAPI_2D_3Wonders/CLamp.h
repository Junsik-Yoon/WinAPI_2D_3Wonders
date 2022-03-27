#pragma once
#include "CItem.h"

class CD2DImage;

class CLamp : public CItem
{
private:
	CD2DImage* m_pImg;
public:
	virtual void update();
	virtual void render();
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	CLamp();
	~CLamp();
};

