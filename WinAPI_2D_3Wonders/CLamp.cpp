#include "pch.h"
#include "CLamp.h"
#include "CCollider.h"
#include "CAnimator.h"

CLamp::CLamp()
{
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


void CLamp::update()
{
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


