#pragma once

class CCollider;

union COLLIDER_ID
{
	struct
	{
		UINT left_id;
		UINT Right_id;
	};
	ULONGLONG ID;
};

class CCollisionManager
{
	SINGLETON(CCollisionManager);

private:

	UINT m_arrCheck[(UINT)GROUP_GAMEOBJ::SIZE];
	map<ULONGLONG, bool> m_mapCollInfo;

	void CollisionGroupUpdate(GROUP_GAMEOBJ objLeft, GROUP_GAMEOBJ objRight);
	bool IsCollision(CCollider* pLeftCollider, CCollider* pRightCollider);

public:
	void init();
	void update();

	void CheckGroup(GROUP_GAMEOBJ objLeft, GROUP_GAMEOBJ objRight);		// 그룹간 충돌 설정
	void UnCheckGroup(GROUP_GAMEOBJ objLeft, GROUP_GAMEOBJ objRight);	// 그룹간 충돌 해제
	void Reset();
};


