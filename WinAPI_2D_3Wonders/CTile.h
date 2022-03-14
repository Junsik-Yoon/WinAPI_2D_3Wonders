#pragma once

#include "CGameObject.h"

class CTexture;
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
	CTexture* m_pTex;
	int m_iIdx;
	CCollider* m_pCollider; //충돌체가있다면
	bool isCollision;
public:
	const static int SIZE_TILE = 32;
public:
	virtual void update();
	virtual void render(HDC hDC);
public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
public:
	void SetTexture(CTexture* pTex) { m_pTex = pTex; }
	void SetTileIdx(int idx) { m_iIdx = idx; }
	int GetTileIdx() { return m_iIdx; }
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

	void SetCollider(bool _isCollision) { isCollision = _isCollision; }
	bool CheckCollider() { return isCollision; }
	iPoint CheckCurrentIndex();
public:
	CTile();
	virtual ~CTile();
	virtual CTile* Clone() { return new CTile(*this); }

};
