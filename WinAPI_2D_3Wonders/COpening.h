#pragma once
#include "CGameObject.h"

class CD2DImage;

class COpening :public CGameObject
{
private:

    CD2DImage* m_pImg;
public:

    virtual void update();
    virtual void render();

public:
    COpening();
    ~COpening();
    COpening* Clone() { return new COpening(*this); }
};

