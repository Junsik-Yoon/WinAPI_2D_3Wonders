#pragma once
#include "CGameObject.h"

class CD2DImage;

enum class eState_Goblin
{
	BORN,		//���� �� -> 1-1����  �ٴڿ����� �������� 1-2������ ���߿����� ���� �ȴ�
	MOVE,		//�����ǰ� �ٷ� �÷��̾�������� �����̰� �ݴ������ ������ �ʴ´�. 
				  //�÷��̾ �ڽ��� �ڿ� ��ġ�ϰ� �� �� �׳� �����ļ� ��������. 
				  //���� �鹫���� ���´�
	LAUGH,		//�÷��̾ �������� �԰ų�, 0.5�������� �ְų� �ؼ� �� ���� ������ ������ ��
	JUMP,		//�÷��̾ �ڽ��� �پ�������� �� ��
	JUMPUP,		//�÷��̾�� x���� ���� �ڽ��� �� Ÿ�Ͽ� ���� ��
	JUMPDOWN,	//�÷��̾�� x���� ���� �ڽ��� �Ʒ� Ÿ�Ͽ� ���� ��
	TELEPORT,	//�÷���Ÿ�Ͽ� �ְ� ���� ������ �÷��̾�� ������ �� ���� ��
	DEAD,		//������� �ϰ�  ����Ʈ�� �Բ� ������

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

