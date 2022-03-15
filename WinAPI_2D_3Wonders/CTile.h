#pragma once

#include "CGameObject.h"

class CD2DImage;
class CCollider;

class CTile : public CGameObject
{
public:
	enum class TYPE
	{
		GROUND,
		WALL,
		DOOR,
		SIZE,
	};

private:
	CD2DImage* m_pImg;
	int m_iIdx;
	CCollider* m_pCollider; //충돌체가있다면
	bool isCollision;
public:
	const static int SIZE_TILE = 32;
public:
	virtual void update();
	virtual void render();
public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
public:
	void SetTexture(CD2DImage* pImg) { m_pImg = pImg; }
	void SetTileIdx(int idx) { m_iIdx = idx; }
	int GetTileIdx() { return m_iIdx; }
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

	void SetCollider(bool _isCollision) { isCollision = _isCollision; }
	bool CheckCollider() { return isCollision; }
public:
	CTile();
	virtual ~CTile();
	virtual CTile* Clone() { return new CTile(*this); }

};
