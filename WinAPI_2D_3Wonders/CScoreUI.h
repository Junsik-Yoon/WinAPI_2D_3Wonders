#pragma once
#include "CUI.h"

class CD2DImage;

class CScoreUI : public CUI
{
private:
	int m_gameScore;
	CD2DImage* m_pImg;
public:
	virtual void update();
	virtual void render();
public:
	CScoreUI();
	virtual ~CScoreUI();
	CScoreUI* Clone() { return  new CScoreUI(*this); }
};

