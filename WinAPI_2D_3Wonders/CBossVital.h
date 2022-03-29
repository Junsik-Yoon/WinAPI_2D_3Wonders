#pragma once
#include "CUI.h"
class CD2DImage;
class CGolemWood;

class CBossVital :public CUI
{
private:
	CD2DImage* m_pImg;
	CGolemWood* m_boss;
	int m_vital;
public:
	int GetVital() { return m_vital; }
public:
	virtual void update();
	virtual void render();
public:
	CBossVital();
	CBossVital(CGolemWood* _boss);
	virtual ~CBossVital();
	CBossVital* Clone() { return  new CBossVital(*this); }
};


