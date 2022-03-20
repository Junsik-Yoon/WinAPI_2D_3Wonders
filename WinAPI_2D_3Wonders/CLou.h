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
    MOVE,
    JUMP,
    SHOOTING,
    JUMPSHOOTING,
    HOLDCLIFF,
    DASH,
    INVINSIBLE,
    FLY,
    GOTHIT,

    
    SIZE
};

class CLou :
    public CGameObject
{
    static float sCountTime;//����
    bool m_counter_toggle;
private:

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
    void update_move();
    void update_animation();
public:
    CLou();
    ~CLou();
    CLou* Clone() { return new CLou(*this); }
};
