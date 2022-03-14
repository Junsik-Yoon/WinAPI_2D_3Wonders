#pragma once

class CGameObject;

class CCollider
{
	friend class CGameObject;

private:
	static UINT s_iID;

	CGameObject* m_pOwner;		// 충돌체를 가지는 게임오브젝트의 주소
	Vec2 m_fptOffsetPos;		// 충돌체의 변위차(수정값)
	Vec2 m_fptFinalPos;			// 충돌체의 최종 위치
	Vec2 m_fptScale;			// 충돌체의 크기
	int m_iColCount;			// 충돌중

	UINT m_iID;					// 충돌체 고유한 ID 값

public:
	CCollider();
	CCollider(const CCollider& other);
	~CCollider();

	CGameObject* GetObj();
	Vec2 GetOffsetPos();
	Vec2 GetFinalPos();
	Vec2 GetScale();

	void SetOffsetPos(Vec2 offsetPos);
	void SetFinalPos(Vec2 finalPos);
	void SetScale(Vec2 scale);
	UINT GetID();

	CCollider& operator=(CCollider& other) = delete;

	void finalupdate();
	void render(HDC hDC);

	// 충돌 시점 함수
	void OnCollision(CCollider* pOther);
	void OnCollisionEnter(CCollider* pOther);
	void OnCollisionExit(CCollider* pOther);
};

