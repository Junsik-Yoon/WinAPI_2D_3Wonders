#pragma once
#include "CGameObject.h"

class CD2DImage;

class CMissile :public CGameObject
{
private:
	Vec2 m_fvDir;
	float m_fSpeed;
	CD2DImage* m_pImgPlayer;
	CD2DImage* m_pImgGW;

public:

	virtual void update();
	virtual void render();

	void SetDir(Vec2 vec);
	void SetDir(float theta);
	void SetSpeed(float speed){ m_fSpeed = speed; }

	void OnCollisionEnter(CCollider* pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	CMissile();
	~CMissile();
	virtual CMissile* Clone() { return new CMissile(*this); }
	
};

