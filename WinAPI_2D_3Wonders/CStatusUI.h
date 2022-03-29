#pragma once
#include "CUI.h"

class CD2DImage;

class CStatusUI :public CUI
{
private:
	CD2DImage* m_pImg;
public:
	virtual void update();
	virtual void render();
public:
	CStatusUI();
	virtual ~CStatusUI();
	CStatusUI* Clone() { return  new CStatusUI(*this); }
};

