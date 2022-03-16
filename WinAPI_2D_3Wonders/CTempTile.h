#pragma once
#include "CGameObject.h"

class CCollider;

class CTempTile: public CGameObject
{

public:
    void OnCollisionEnter(CCollider* _pOther);
    void OnCollision(CCollider* _pOther);
    void OnCollisionExit(CCollider* _pOther);
public:
    virtual void update();
    virtual void render();

public:
    CTempTile();
    ~CTempTile();
    CTempTile* Clone() { return new CTempTile(*this); }
};

