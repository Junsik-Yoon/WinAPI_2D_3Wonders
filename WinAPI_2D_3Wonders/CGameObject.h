#pragma once

class CCollider;
class CAnimator;

class CGameObject
{
	friend class CEventManager;
private:
	wstring m_strName;	//이름
	Vec2 m_fptPos;		//좌표
	Vec2 m_fptScale;	//크기

	int m_hp;

	bool m_bAlive;

	CCollider* m_pCollider;
	CAnimator* m_pAnimator;

private:
	void SetDead() { m_bAlive = false; }

public:
	virtual void update() = 0;
	virtual void finalupdate();
	virtual void render() = 0;
	virtual void component_render();

	void SetPos(Vec2 pos) { m_fptPos = pos; }
	void SetScale(Vec2 scale) { m_fptScale = scale; }
	Vec2 GetPos() { return m_fptPos; }
	Vec2 GetScale() { return m_fptScale; }
	void SetName(wstring name) { m_strName = name; }
	wstring GetName() { return m_strName; }

	void SetHP(int _hp) { m_hp = _hp; }
	int GetHP() { return m_hp; }

	bool isDead() { return !m_bAlive; }

	CCollider* GetCollider() { return m_pCollider; }
	void CreateCollider();

	virtual void OnCollision(CCollider* _pOther) {}
	virtual void OnCollisionEnter(CCollider* _pOther) {}
	virtual void OnCollisionExit(CCollider* _pOther) {}

	CAnimator* GetAnimator() { return m_pAnimator; }
	void CreateAnimator();
public:
	CGameObject();
	CGameObject(const CGameObject& other);
	virtual ~CGameObject();
	virtual CGameObject* Clone() = 0;
};