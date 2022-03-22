#pragma once
#include "CGameObject.h"

class CD2DImage;

class CHalfMoon : public CGameObject
{
private:
	CD2DImage* m_pImg;

	bool isRight;

public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	void SetRight(bool _isRight) { isRight = _isRight; }
	
public:
	virtual void update();
	virtual void render();
	void render_information();

public:
	CHalfMoon();
	~CHalfMoon();
	CHalfMoon* Clone() { return new CHalfMoon(*this); }
};

