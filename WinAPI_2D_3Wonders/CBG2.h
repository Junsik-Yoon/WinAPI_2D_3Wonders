#pragma once
#include "CGameObject.h"

class CD2DImage;

class CBG2 : public CGameObject
{

private:
	CD2DImage* m_pImg;
	CD2DImage* m_pImg_back;
	int index;
public:
	void setIndex(int _index) { index = _index; }

public:

	virtual void update();
	virtual void render();

public:
	CBG2();
	~CBG2();
	virtual CBG2* Clone() { return new CBG2(*this); }
};

