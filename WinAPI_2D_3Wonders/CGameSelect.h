#pragma once
#include "CGameObject.h"

class CD2DImage;

class CGameSelect :public CGameObject
{
public:
    CD2DImage* m_pImg;

    virtual void update();
    virtual void render();
    bool isSelected;
    float selectCounter;
    UINT select;

public:
    CGameSelect();
    ~CGameSelect();
    CGameSelect* Clone() { return new CGameSelect(*this); }
};

