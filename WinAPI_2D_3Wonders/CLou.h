#pragma once
#include "CGameObject.h"

class CD2DImage;
enum class D_FACING
{
    UP = 1,     //1
    DOWN,       //2
    RIGHT,      //3
    LEFT,       //4
};
enum class eState
{
    IDLE,       //0
    SIT,        //1
    LANDMOVE,   //2
    FALL,       //3
    JUMP,       //4
    DASH,       //5
    GOTHIT,     //6
    DEAD,       //7
    ATTACK,     //8
    HOLDCLIFF,  //9
    INVINCIBLE, //10
    FLY,        //11
  
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

    map<int,wstring> m_stateText;

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
    eState GetState() {return m_state;}
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
    void render_information();

public:
    CLou();
    ~CLou();
    CLou* Clone() { return new CLou(*this); }
};
