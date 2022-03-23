#pragma once

class CGameObject;
class CAnimation;
class CTexture;
class CEffect;

class CAnimator
{
	friend class CGameObject;

private:
	map<wstring, CAnimation*>	m_mapAni;
	map<wstring, CEffect*> m_mapEffect;
	CAnimation* m_pCurAni;
	CGameObject* m_pOwner;
	Vec2 offset;

public:
	CGameObject* GetObj() { return m_pOwner; }

	void update();
	void render();
	
public:

	void CreateAnimation(const wstring& strName, CD2DImage* img,
		Vec2 lt, Vec2 slice, Vec2 step, float duration, 
		UINT frmCount, bool reverse, bool loop=true);
	CAnimation* FindAnimation(const wstring& strName);
	void Play(const wstring& strName);

	CAnimation* GetCurrentAnimation() { return m_pCurAni; }
public:
	CAnimator();
	~CAnimator();
};


