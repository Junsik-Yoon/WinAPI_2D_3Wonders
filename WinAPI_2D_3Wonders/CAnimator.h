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
	void render();
	
	void CreateAnimation(const wstring& strName, CD2DImage* img,
		Vec2 lt, Vec2 slice, Vec2 step, float duration, 
		UINT frmCount, bool reverse);
	CAnimation* FindAnimation(const wstring& strName);
	void Play(const wstring& strName);


public:
	CAnimator();
	~CAnimator();
};


