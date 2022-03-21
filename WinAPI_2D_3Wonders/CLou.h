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
enum class eState
{
    IDLE,
    SIT,
    AIRMOVE,
    LANDMOVE,
    FALL,
    JUMP,
    SHOOTING,
    DEAD,
    LOOKUP,

    JUMPSHOOTING,
    HOLDCLIFF,
    DASH,
    INVINCIBLE,
    FLY,
    GOTHIT,


    
    SIZE
};

class CLou :
    public CGameObject
{
    static float sCountTime;//¹ü¿ë
    bool m_counter_toggle;
private:

    bool bGravity;
    float dash;
    float m_goblinCounter;

    bool isFacedRight;



    D_FACING m_facing;
    eState m_state;
    CD2DImage* m_pImg;
    UINT m_floor;
    UINT m_wall;
    float m_velocity;
    float m_upforce;
    float m_gravity;
    bool isUpside;
    bool isAir;

    int prevHP;
    float prevY;

public:
    void CreateMissile();
    void GenerateGoblin();
public:
    void OnCollisionEnter(CCollider* _pOther);
    void OnCollision(CCollider* _pOther);
    void OnCollisionExit(CCollider* _pOther);
public:
    virtual void update();
    virtual void render();
public:
    eState GetState() { return m_state; }
public:
    void update_move();
    void update_animation();
public:
    CLou();
    ~CLou();
    CLou* Clone() { return new CLou(*this); }
};
