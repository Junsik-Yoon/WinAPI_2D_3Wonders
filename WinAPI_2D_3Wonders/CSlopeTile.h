#pragma once
#include "CTile.h"


class CSlopeTile : public CTile
{
public:
    static void create(float left, float top, float right, float bottom,GROUP_TILE group);
    void OnCollisionEnter(CCollider* _pOther);
    void OnCollision(CCollider* _pOther);
    void OnCollisionExit(CCollider* _pOther);
public:
    virtual void update();
    virtual void render();

public:
    CSlopeTile();
    ~CSlopeTile();
};

