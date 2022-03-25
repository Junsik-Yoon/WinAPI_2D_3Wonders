#pragma once
#include "CGameObject.h"

class CD2DImage;
class CBug;

enum class eState_HM
{
	IDLE,
	SUMMON,
	DEAD,

	SIZE,
};

class CHalfMoon : public CGameObject
{
private:
	vector<CBug*> pBugs;
	CD2DImage* m_pImg;

	bool isRight;
	bool isDead;
	eState_HM m_state;

	bool isProducing;
	float bugTimer;
	int bugCounter;
	float m_summonTimer;


public:
	void GenerateBug();
	bool BugLivingCheck();

public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
public:
	void SetRight(bool _isRight) { isRight = _isRight; }
	
public:
	virtual void update();
	virtual void render();
	void render_information();

public:
	CHalfMoon();
	~CHalfMoon();
	CHalfMoon* Clone() { return new CHalfMoon(*this); }
};

