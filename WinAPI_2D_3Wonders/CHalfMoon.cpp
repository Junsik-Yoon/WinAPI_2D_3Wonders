#include "pch.h"
#include "CHalfMoon.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CBug.h"

CHalfMoon::CHalfMoon()
{
	bugCounter = 0;
	CBug* pBug = nullptr;
	for (int i = 0; i < 4; ++i)
	{
		pBug = new CBug();
		pBugs.push_back(pBug);
		pBugs[i]->SetHP(0);
		pBugs[i]->SetPos(Vec2(-500.f, 0.f));
		CreateObj(pBug, GROUP_GAMEOBJ::MONSTER);
	}

	bugTimer = 0.f;
	m_summonTimer = 0.f;
	SetHP(8);
	isRight = true;
	isProducing = false;
	SetName(L"Halfmoon");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"HalfmoonBugImg", L"texture\\Animation\\Animation_HalfmoonBug.png");

	CreateCollider();
	SetScale(Vec2(60.f, 100.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));


	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Idle_Right", m_pImg, Vec2(0.f, 128.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Idle_Left", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);

	GetAnimator()->CreateAnimation(L"Summon_Right", m_pImg, Vec2(128.f, 128.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 9, false);
	GetAnimator()->CreateAnimation(L"Summon_Left", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 9, false);

	GetAnimator()->CreateAnimation(L"Dead_Left" ,m_pImg, Vec2(1280.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Dead_Right", m_pImg, Vec2(1280.f, 128.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);


	GetAnimator()->Play(L"Idle_Right");

	CCameraManager::getInst()->GetRenderPos(GetPos());

}

CHalfMoon::~CHalfMoon()
{
}

void CHalfMoon::update()
{
	Vec2 vPos = GetPos();
	
	if (GetHP())
	{
		if (false == BugLivingCheck())
		{
			isProducing = true;
		}
		if (isProducing)
		{
			m_summonTimer += fDT;
		}
		if (m_summonTimer >= 3.f)
		{
			GenerateBug();
			if (m_summonTimer >= 7.f)
			{
				m_summonTimer = 0.f;
			}
		}
		if (4 == bugCounter && !BugLivingCheck())
		{
			bugCounter = 0;
		}
		for (int i = 0; i < 4; ++i)
		{
			if (0 >= pBugs[i]->GetHP())
			{
				pBugs[i]->SetPos((Vec2(-500.f, 0.f)));
			}
		}
	}
	else
	{
		for (int i = 0; i < pBugs.size(); ++i)
		{
			DeleteObj(pBugs[i]);//TODO: 삭제에러
		}
		pBugs.clear();
	}
	
	SetPos(vPos);

	GetAnimator()->update();
}

void CHalfMoon::render()
{
	component_render();
	render_information();
}

void CHalfMoon::render_information()
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
			L" HP:  " + std::to_wstring(GetHP()) + L"\n"
			, vPos.x + 30.f
			, vPos.y + -40.f
			, vPos.x + 200.f
			, vPos.y + 100.f
			, 16.f
			, RGB(255, 255, 255));
	}
}


void CHalfMoon::GenerateBug()
{
	vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	Vec2 vPlayerPos = pPlayer[0]->GetPos();
	Vec2 vPos = GetPos();
	for (int i = 0; i < pBugs.size(); ++i)
	{
		if (vPlayerPos.x < vPos.x)
		{
			//pBugs[i]->SetHeadRight = false;
		}
	}

	bugTimer += fDT;

	if (bugTimer >= 1.f)
	{
		if (0== bugCounter)
		{
			++bugCounter;
			bugTimer = 0.f;
			pBugs[0]->SetPos(Vec2(vPos));
			pBugs[0]->SetHP(1);
			pBugs[0]->SetState(eState_Bug::FLY);
		}
		else if (1== bugCounter)
		{
			++bugCounter;
			bugTimer = 0.f;
			pBugs[1]->SetPos(Vec2(vPos));
			pBugs[1]->SetHP(1);
			pBugs[1]->SetState(eState_Bug::FLY);
		}
		else if (2== bugCounter)
		{
			++bugCounter;
			bugTimer = 0.f;
			pBugs[2]->SetPos(Vec2(vPos));
			pBugs[2]->SetHP(1);
			pBugs[2]->SetState(eState_Bug::FLY);
		}
		else if (3== bugCounter)
		{
			++bugCounter;
			bugTimer = 0.f;
			pBugs[3]->SetPos(Vec2(vPos));
			pBugs[3]->SetHP(1);
			pBugs[3]->SetState(eState_Bug::FLY);
		}
	
	}

}

bool CHalfMoon::BugLivingCheck()
{
	if (0 >= pBugs[0]->GetHP() &&
		0 >= pBugs[1]->GetHP() &&
		0 >= pBugs[2]->GetHP() &&
		0 >= pBugs[3]->GetHP())
	{
		return false;
	}
	return true;
}

void CHalfMoon::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	Vec2 vPlayerPos = pOther->GetPos();
	int iPlayerHP = pOther->GetHP();
	if (pOther->GetName() == L"Lou")
	{
		if (_pOther->GetFinalPos().x < GetCollider()->GetFinalPos().x)
		{
			vPlayerPos.x -= 2.f;
		}
		if (_pOther->GetFinalPos().x > GetCollider()->GetFinalPos().x)
		{
			vPlayerPos.x += 2.f;
		}
		pOther->SetHP(--iPlayerHP);
	}
	if (pOther->GetName() == L"Missile_Player")
	{
		int hp = GetHP();
		SetHP(--hp);
	}
	if (GetHP() <= 0)
	{
		if (isRight)
		{
			GetAnimator()->Play(L"Dead_Right");
		}
		else
		{
			GetAnimator()->Play(L"Dead_Left");
		}
		GetCollider()->SetOffsetPos(Vec2(0.f, -500.f));
	}

}

void CHalfMoon::OnCollision(CCollider* _pOther)
{
}

void CHalfMoon::OnCollisionExit(CCollider* _pOther)
{

}

