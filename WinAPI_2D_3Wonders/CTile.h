#pragma once

#include "CGameObject.h"

class CD2DImage;

class CTile : public CGameObject
{
private:
	CD2DImage* m_pImg;
	int m_iX;
	int m_iY;
	int m_iIdx;

	GROUP_TILE m_group;

public:
	const static int SIZE_TILE = 32;
public:
	virtual void update();
	virtual void render();

public:
	void SetD2DImage(CD2DImage* pImg) { m_pImg = pImg; }

	void SetImgIdx(int idx) { m_iIdx = idx; }
	int GetIdx() { return m_iIdx; }
	
	void SetX(int x) { m_iX = x; }
	void SetY(int y) { m_iY = y; }
	int GetX() { return m_iX; }
	int GetY() { return m_iY; }

	void SetGroup(GROUP_TILE group){ m_group = group; }
	GROUP_TILE GetGroup() { return m_group; }

	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	CTile();
	virtual ~CTile();
	virtual CTile* Clone() { return new CTile(*this); }

};
