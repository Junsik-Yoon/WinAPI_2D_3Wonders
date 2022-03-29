#include "pch.h"
#include "CLamp.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CD2DImage.h"
#include "CEffect.h"

#include <iostream>
#include <random>

CLamp::CLamp()
{
	twinkle = 0.f;
	SetName(L"Lamp");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"LampImg", L"texture\\lamp.png");
	CreateCollider();
	SetScale(Vec2(30.f, 30.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));

	CreateAnimator();

	GetAnimator()->CreateAnimation(L"LampImg", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.5f, 1, false,false);
	GetAnimator()->Play(L"LampImg");
}

CLamp::~CLamp()
{

}


void CLamp::TwinkleEffect()
{
	int iHeight = m_pImg->GetHeight();
	int iWidth = m_pImg->GetWidth();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, iWidth/2.f);

	//////////////////////ÀÌÆåÆ®///////////////
	CEffect* effectTwinkle = new CEffect(L"Effect_Twinkle", L"texture\\Animation\\Effect_Twinkle.png",
		L"Effect_Twinkle", Vec2(0.f, 0.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 0.05f, 6, false, 1.f);
	effectTwinkle->SetPos(Vec2(GetPos().x - GetScale().x  + dis(gen), GetPos().y - GetScale().y + dis(gen)));
	effectTwinkle->SetDuration(1.f);
	CreateObj(effectTwinkle, GROUP_GAMEOBJ::EFFECT);
	///////////////////////////////////////////
}

void CLamp::update()
{
	twinkle += fDT;
	if (twinkle >= 0.1f)
	{
		twinkle = 0.f;
		TwinkleEffect();
	}

	GetAnimator()->update();
}

void CLamp::render()
{

	component_render();

}			

void CLamp::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Lou")
	{
		DeleteObj(this);
	}
}

void CLamp::OnCollision(CCollider* _pOther)
{
}

void CLamp::OnCollisionExit(CCollider* _pOther)
{
}


