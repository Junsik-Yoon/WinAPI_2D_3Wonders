#pragma once
#include "CGameObject.h"

class CD2DImage;

class CLou :
    public CGameObject
{
    static float sCountTime;//¹ü¿ë
    bool m_counter_toggle;
private:
    CD2DImage* m_pImg;
    UINT m_floor;
    UINT m_wall;
    float m_velocity;
    float m_upforce;
    float m_gravity;
    bool isUpside;
    bool isAir;
    bool isFacedRight;

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);
public:
    virtual void update();
    virtual void render();

public:
    CLou();
    ~CLou();
    CLou* Clone() { return new CLou(*this); }
};
