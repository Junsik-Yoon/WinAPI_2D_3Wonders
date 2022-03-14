#pragma once

class CGameObject;
class CAnimation;
class CTexture;

class CAnimator
{
	friend class CGameObject;

private:
	map<wstring, CAnimation*>	m_mapAni;
	CAnimation* m_pCurAni;
	CGameObject* m_pOwner;

public:
	CGameObject* GetObj() { return m_pOwner; }

	void update();
	void render(HDC hDC);

	void CreateAnimation(const wstring& strName, CTexture* tex,
		Vec2 lt, Vec2 slice, Vec2 step, float duration, UINT frmCount);
	CAnimation* FindAnimation(const wstring& strName);
	void Play(const wstring& strName);


public:
	CAnimator();
	~CAnimator();
};


