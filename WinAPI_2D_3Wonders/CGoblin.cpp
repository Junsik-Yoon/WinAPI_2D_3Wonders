#include "pch.h"
#include "CGoblin.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#define D_VELOCITY 150
#define D_GRAVITY 400

CGoblin::CGoblin()
{
	SetHP(2);
	m_floor=0;
	m_wall=0;
	m_velocity = D_VELOCITY;
	m_gravity = D_GRAVITY;

	SetName(L"Goblin");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"GoblinImg", L"texture\\Animation\\Animation_Goblin.png");
	CreateCollider();
	SetScale(Vec2(60.f, 80.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 13.f));

	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Create_Left", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6,false);
	GetAnimator()->CreateAnimation(L"Create_Right", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6,false);
	
	GetAnimator()->CreateAnimation(L"Move_Left", m_pImg, Vec2(256.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6, false);
	GetAnimator()->CreateAnimation(L"Move_Right", m_pImg, Vec2(384.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6, false);
	
	GetAnimator()->CreateAnimation(L"Laugh_Left", m_pImg, Vec2(128.f, 896.f), Vec2(256.f, 256.f), Vec2(256.f, 0.f), 0.3f, 2, false);
	GetAnimator()->CreateAnimation(L"Laugh_Right", m_pImg, Vec2(128.f, 1152.f), Vec2(256.f, 256.f), Vec2(256.f, 0.f), 0.3f, 2, false);

	GetAnimator()->CreateAnimation(L"Die_Left", m_pImg, Vec2(640.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 5, false);
	GetAnimator()->CreateAnimation(L"Die_Right", m_pImg, Vec2(512.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 5, false);


	GetAnimator()->Play(L"Die_Left");
	CCameraManager::getInst()->GetRenderPos(GetPos());
}

CGoblin::~CGoblin()
{
}


void CGoblin::update()
{
	Vec2 vPos = GetPos();

	if (m_floor == 0)
	{
		m_gravity = D_GRAVITY;
		vPos.y += D_GRAVITY * fDT;
	}
	else
	{
		m_gravity = 0;
	}

	vector<CGameObject*> pPlayer=CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	if (pPlayer[0]->GetPos().x >= GetPos().x)
	{
		vPos.x += 50.f * fDT;
		GetAnimator()->Play(L"Move_Right");
	}
	else
	{
		vPos.x -= 50.f * fDT;
		GetAnimator()->Play(L"Move_Left");
	}
	//TODO: �÷��̾�� ��ǥ x����(���밪)�� ���� �����̰� y���� ���� �Ʒ��� ��� ����� �����ؼ� �÷��̾ ��ų� �����ؼ� �������� �� ����

	SetPos(vPos);

	GetAnimator()->update();
}

void CGoblin::render()
{
	component_render();
	render_information();
}

void CGoblin::render_information()
{
	if (true == CCore::getInst()->DebugMode())
	{
		CD2DImage* pImg = CResourceManager::getInst()->LoadD2DImage(L"BackInfo", L"texture\\BackInfo.png");
		Vec2 vPos = GetPos();
		vPos = CCameraManager::getInst()->GetRenderPos(vPos);

		CRenderManager::getInst()->RenderImage(
			pImg,
			vPos.x + 30.f,
			vPos.y + -40.f,
			vPos.x + 200.f,
			vPos.y + 100.f,
			0.3f);

		////////////////////////
		wstring curAni = {};
		////////////////////////
		curAni = GetAnimator()->GetCurrentAnimation()->GetName();
		CRenderManager::getInst()->RenderText(
			L" pos X : " + std::to_wstring(GetPos().x) + L"\n" +
			L" pos Y : " + std::to_wstring(GetPos().y) + L"\n" +
			L" state  : " + L"" + L"\n" +
			L" drctn  : " + std::to_wstring(isFacedRight) + L"\n" +//TODO:����
			L" curAm : " + curAni + L"\n" +
			L" HP:  " + std::to_wstring(GetHP()) + L"\n" +
			L" wallCount : " + std::to_wstring(m_floor)
			, vPos.x + 30.f
			, vPos.y + -40.f
			, vPos.x + 200.f
			, vPos.y + 100.f
			, 16.f
			, RGB(255, 255, 255));
	}
}

void CGoblin::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		++m_floor;
	}
	if (pOther->GetName() == L"Lou")
	{
		if (isFacedRight)
		{
			GetAnimator()->Play(L"Laugh_Right");
		}//TODO:Ÿ�̸ӳ� �ٸ� ���� ����ؼ� �� ���� �����ൿ�ϰ��ϱ�
		else
		{
			GetAnimator()->Play(L"Laugh_Left");
		}
	}
	if (pOther->GetName() == L"Missile_Player")
	{
		int hp = GetHP();
		SetHP(--hp);
	}
	if (GetHP() <= 0)
	{
		DeleteObj(this);
	}
}

void CGoblin::OnCollision(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	Vec2 vPos = GetPos();
	if (pOther->GetName() == L"Tile")
	{
		int a = abs((int)(GetCollider()->GetFinalPos().y - _pOther->GetFinalPos().y));
		int b = (int)(GetCollider()->GetScale().y / 2.f + _pOther->GetScale().y / 2.f);
		int sum = abs(a - b);
		if(1<sum)
			--vPos.y;
	}
	
	SetPos(vPos);
}

void CGoblin::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		--m_floor;
	}
}





