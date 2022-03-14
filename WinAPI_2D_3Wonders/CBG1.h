#pragma once
#include "CGameObject.h"

class CBG1 : public CGameObject
{

private:
	CTexture* m_pTex;

public:

	virtual void update();
	virtual void render(HDC hDC);

public:
	CBG1();
	~CBG1();
	virtual CBG1* Clone() { return new CBG1(*this); }
};
