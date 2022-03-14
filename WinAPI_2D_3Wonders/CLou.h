#pragma once
#include "CGameObject.h"

class CTexture;

class CLou :
    public CGameObject
{

private:
    CTexture* m_pTex;
    UINT m_floor;
    UINT m_wall;
    float m_velocity;
    float m_upforce;
    float m_gravity;
    bool isJump;

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);
public:
    virtual void update();
    virtual void render(HDC hDC);

public:
    CLou();
    ~CLou();
    CLou* Clone() { return new CLou(*this); }
};
