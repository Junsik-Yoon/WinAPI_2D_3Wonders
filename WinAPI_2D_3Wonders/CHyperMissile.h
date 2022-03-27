#pragma once
#include "CGameObject.h"

class CD2DImage;

class CHyperMissile : public CGameObject
{
private:
	Vec2 m_fvDir;
	float m_fSpeed;
	CD2DImage* m_pImgHyper;
public:

	virtual void update();
	virtual void render();
public:
	Vec2 GetDir() { return m_fvDir; }
	void SetDir(Vec2 vec);
	void SetDir(float theta);
	void SetSpeed(float speed) { m_fSpeed = speed; }
public:
	void OnCollisionEnter(CCollider* pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	CHyperMissile();
	CHyperMissile(Vec2 _dirVec);
	~CHyperMissile();
	virtual CHyperMissile* Clone() { return new CHyperMissile(*this); }

};

