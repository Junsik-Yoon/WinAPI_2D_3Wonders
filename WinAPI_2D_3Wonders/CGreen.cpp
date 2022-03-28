#include "pch.h"
#include "CGreen.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CShootFire.h"
#include "CLou.h"

#define D_VELOCITY 150
#define D_GRAVITY 400
CGreen::CGreen()
{
	SetHP(10);
	m_shootFire = 0.f;
	isShootingFire = 0;
	isRight = false;
	m_state = eState_Green::IDLE;
	m_floor = 0;
	m_wall = 0;
	m_velocity = D_VELOCITY;
	m_gravity = D_GRAVITY;

	SetName(L"Green");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"GreenImg", L"texture\\Animation\\Animation_Green.png");
	CreateCollider();
	SetScale(Vec2(120.f, 120.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 20.f));

	CreateAnimator();

	GetAnimator()->CreateAnimation(L"Move_Left", m_pImg, Vec2(0.f, 0.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.2f, 5,false);
	GetAnimator()->CreateAnimation(L"Move_Right", m_pImg, Vec2(0.f, 384.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.2f, 5,false);

	GetAnimator()->CreateAnimation(L"Fire_Right", m_pImg, Vec2(0.f, 576.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.2f, 2, false,false);
	GetAnimator()->CreateAnimation(L"Fire_Left", m_pImg, Vec2(0.f, 192.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.2f, 2, false,false);



	GetAnimator()->Play(L"Move_Left");

	CSoundManager::getInst()->AddSound(L"fireshootingSound", L"sound\\fireshootingSound.wav", false);
	

	
}

CGreen::~CGreen()
{
}


void CGreen::update()
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

	vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	CLou* pLou = (CLou*) pPlayer[0];
	if (abs(pPlayer[0]->GetPos().x-GetPos().x)<400.f
		&& abs(pPlayer[0]->GetPos().x - GetPos().x) > 200.f)//idle->trace로 변경
	{
		if (pPlayer[0]->GetPos().x >= GetPos().x)
		{
			isRight = true;
			vPos.x += 50.f * fDT;
			GetAnimator()->Play(L"Move_Right");
		}
		else
		{
			isRight = false;
			vPos.x -= 50.f * fDT;
			GetAnimator()->Play(L"Move_Left");
		}
	}
	if (abs(pPlayer[0]->GetPos().x - GetPos().x) <= 200.f
		&& (pLou->GetState() != eState::DEAD||
			pLou->GetState() != eState::GOTHIT))//trace->attack으로변경
	{
		
		m_shootFire += fDT;

		if (pPlayer[0]->GetPos().x >= GetPos().x)
		{
			isRight = true;
		}
		else
		{
			isRight = false;
		}
		if (m_shootFire >= 0.2f)
		{
			++isShootingFire;
			if (isShootingFire >= 5)
			{
				isShootingFire = 0;
			}
			ShootFire();
			CSoundManager::getInst()->Play(L"fireshootingSound",0.2f);
			m_shootFire = 0.f;
		}
	}

	SetPos(vPos);
	GetAnimator()->update();
}

void CGreen::render()
{
	component_render();
	render_information();
}

void CGreen::render_information()
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
			L" drctn  : " + std::to_wstring(isRight) + L"\n" +//TODO:수정
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


void CGreen::ShootFire()
{
	CShootFire* pShootFire = new CShootFire();
	Vec2 fptFirePos = GetPos();
	pShootFire->SetName(L"GreenFire");
	pShootFire->SetStartPos(fptFirePos);
	pShootFire->SetPos(fptFirePos);
	pShootFire->SetScale(Vec2(64.f, 64.f));
	pShootFire->GetCollider()->SetScale(Vec2(pShootFire->GetScale().x, pShootFire->GetScale().y));
	
	if (isRight)
	{
		GetAnimator()->Play(L"Fire_Right");
		pShootFire->SetRight(true);
		pShootFire->SetPos(Vec2(fptFirePos.x + 55.f, fptFirePos.y + 20.f));
		pShootFire->SetDir(Vec2(1.f, 0.f));
	}
	else
	{
		//TODO:방향안먹는거수정
		GetAnimator()->Play(L"Fire_Left");
		pShootFire->SetRight(false);
		pShootFire->SetPos(Vec2(fptFirePos.x-55.f, fptFirePos.y + 20.f));
		pShootFire->SetDir(Vec2(-1.f, 0.f));
	}
	pShootFire->GetAnimator()->Play(L"GreenFire");
	CreateObj(pShootFire, GROUP_GAMEOBJ::FIRE);
}

void CGreen::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		++m_floor;
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

void CGreen::OnCollision(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	Vec2 vPos = GetPos();
	if (pOther->GetName() == L"Tile")
	{
		int a = abs((int)(GetCollider()->GetFinalPos().y - _pOther->GetFinalPos().y));
		int b = (int)(GetCollider()->GetScale().y / 2.f + _pOther->GetScale().y / 2.f);
		int sum = abs(a - b);
		if (1 < sum)
			--vPos.y;
	}

	SetPos(vPos);
}

void CGreen::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		--m_floor;
	}
}





