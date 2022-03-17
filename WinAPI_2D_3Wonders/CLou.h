#pragma once
#include "CGameObject.h"

class CD2DImage;
enum class D_FACING
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
    SIZE
};
enum class dState
{
    ONLAND,
    UPAIR,
    DOWNAIR,
    U_ONLAND,
    U_UPAIR,
    U_DOWNAIR,
    



    SIZE
};

class CLou :
    public CGameObject
{
    static float sCountTime;//¹ü¿ë
    bool m_counter_toggle;
private:

    D_FACING m_facing;
    UINT m_state[(UINT)dState::SIZE];
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
    void CreateMissile();

public:
    void OnCollisionEnter(CCollider* _pOther);
    void OnCollision(CCollider* _pOther);
    void OnCollisionExit(CCollider* _pOther);
public:
    virtual void update();
    virtual void render();
    
public:
    CLou();
    ~CLou();
    CLou* Clone() { return new CLou(*this); }
};
