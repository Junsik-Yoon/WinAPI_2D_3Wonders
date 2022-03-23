#include "pch.h"
#include "CEffect.h"
#include "CAnimator.h"
#include "CCollider.h"

CEffect::CEffect(const wstring& imgName, const wstring& imgPath, const wstring& aniName,
	Vec2 lt, Vec2 slice, Vec2 step, float duration,
	UINT frmCount, bool reverse,float entire_dur, bool loop, const wstring& effName)
{
	SetDuration(entire_dur);
	SetScale(Vec2(50.f, 50.f));

	m_pImg = CResourceManager::getInst()->LoadD2DImage(imgName, imgPath);
	CreateAnimator();
	GetAnimator()->CreateAnimation(aniName, m_pImg,
		lt, slice, step, duration, frmCount, reverse, loop);

	SetName(effName);
	GetAnimator()->Play(aniName);
	CCameraManager::getInst()->GetRenderPos(GetPos());
}

CEffect::~CEffect()
{
}

void CEffect::update()
{
	m_duration -= fDT;
	if (m_duration <= 0.f)
	{
		DeleteObj(this);
	}
	GetAnimator()->update();
}

void CEffect::render()
{
	component_render();
}



