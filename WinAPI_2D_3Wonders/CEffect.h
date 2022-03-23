#pragma once
#include "CGameObject.h"

class CD2DImage;

class CEffect : public CGameObject
{
	friend class CAnimator;
private:
	CD2DImage* m_pImg;
	float m_duration;
public:
	void SetDuration(float _duration) { m_duration = _duration; }
public:
	virtual void update();
	virtual void render();
public:
	CEffect(const wstring& imgName, const wstring& imgPath, const wstring& aniName,
		Vec2 lt, Vec2 slice, Vec2 step, float duration,
		UINT frmCount, bool reverse, float entire_dur, 
		bool loop = true, const wstring& effName = L"Effect");
	~CEffect();
	CEffect* Clone() { return new CEffect(*this); }
};

