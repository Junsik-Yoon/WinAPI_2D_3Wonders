#pragma once
#include "CItem.h"

class CD2DImage;

class CCoin : public CItem
{
private:
	CD2DImage* m_pImg;
	float create_motion;
public:
	virtual void update();
	virtual void render();
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	CCoin();
	~CCoin();
	//CCoin* Clone() { return new CCoin(*this); }
};

