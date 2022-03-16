#pragma once
#include "CGameObject.h"
class CD2DImage;

class CBG1 : public CGameObject
{

private:
	CD2DImage* m_pImg;

public:

	virtual void update();
	virtual void render();

public:
	CBG1();
	~CBG1();
	virtual CBG1* Clone() { return new CBG1(*this); }
};
