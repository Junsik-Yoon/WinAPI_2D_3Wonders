#pragma once
#include "CGameObject.h"

class CD2DImage;

enum class eState_Goblin
{
	BORN,		//리젠 시 -> 1-1에선  바닥에서만 젠되지만 1-2에서는 공중에서도 젠이 된다
	MOVE,		//리젠되고 바로 플레이어방향으로 움직이고 반대방향을 가지지 않는다. 
				  //플레이어가 자신의 뒤에 위치하게 될 시 그냥 지나쳐서 지나간다. 
				  //가끔 백무빙을 섞는다
	LAUGH,		//플레이어가 데미지를 입거나, 0.5데미지를 주거나 해서 한 번에 죽이지 못했을 시
	JUMP,		//플레이어가 자신을 뛰어넘으려고 할 시
	JUMPUP,		//플레이어와 x축이 같고 자신의 위 타일에 있을 시
	JUMPDOWN,	//플레이어와 x축이 같고 자신의 아래 타일에 있을 시
	TELEPORT,	//플랫폼타일에 있고 끝이 막혀서 플레이어에게 도달할 수 없을 시
	DEAD,		//백덤블링을 하고  이펙트와 함께 터진다

	SIZE,
};

class CGoblin : public CGameObject
{
private:
	CD2DImage* m_pImg;
	eState_Goblin m_state;
	UINT m_floor;
	UINT m_wall;
	float m_velocity;
	float m_gravity;
	bool isFacedRight;
	float m_timeCounter;
	bool isBackflipRight;
	bool isEffectOn;
	int prevPlayerHP;
public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	virtual void update();
	virtual void render();
	void update_move();
	void update_animation();
	void render_information();

public:
	void SetFacedRight(bool facingRight) { isFacedRight = facingRight; }
	void SetBackflipDir(bool _isBfRight) { isBackflipRight = _isBfRight; }
public:
	CGoblin();
	~CGoblin();
	CGoblin* Clone() { return new CGoblin(*this); }
};

