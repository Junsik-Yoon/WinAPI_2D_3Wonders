#pragma once
#include "CTile.h"

class CD2DImage;

class CShelter :public CTile
{

private:
	CD2DImage* m_pImg;
	GROUP_TILE m_eGroup;
public:
	virtual void update();
	virtual void render();
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);

public:
	CShelter();
	~CShelter();
};
