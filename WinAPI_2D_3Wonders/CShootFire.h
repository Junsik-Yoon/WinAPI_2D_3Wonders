#pragma once
#include "CMissile.h"

class CD2DImage;

class CShootFire : public CMissile
{
private:

	float vanishToggle;
	float shootRange;

	Vec2 startPos;

	bool isRight;
	Vec2 m_fvDir;
	float m_fSpeed;
	CD2DImage* m_pImg;

public:
	virtual void update();
	virtual void render();

	void SetRight(bool _isRight) { isRight = _isRight; }
	void SetStartPos(Vec2 _pos) { startPos = _pos; }
public:
	void OnCollisionEnter(CCollider* pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);

public:
	CShootFire();
	~CShootFire();

};

