#include "pch.h"
#include "CHalfMoon.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CBug.h"
#include "CEffect.h"

CHalfMoon::CHalfMoon()
{
	CBug* pBug = nullptr;
	for (int i = 0; i < bugPool; ++i)
	{
		pBug = new CBug();
		pBugs.push_back(pBug);
		pBugs[i]->SetHP(0);
		pBugs[i]->SetPos(Vec2(-500.f, 0.f));
		CreateObj(pBug, GROUP_GAMEOBJ::MONSTER);
	}


	isDead = false;
	bugCounter = 0;
	bugTimer = 0.f;
	m_summonTimer = 0.f;
	SetHP(8);
	isRight = true;
	isProducing = false;
	SetName(L"Halfmoon");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"HalfmoonBugImg", L"texture\\Animation\\Animation_HalfmoonBug.png");
	m_pWhiteImg = CResourceManager::getInst()->LoadD2DImage(L"HalfmoonWhiteImg", L"texture\\Animation\\Animation_halfwhite.png"); 
	CreateCollider();
	SetScale(Vec2(60.f, 100.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));


	CreateAnimator();

	GetAnimator()->CreateAnimation(L"White_Right", m_pWhiteImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"White_Left", m_pWhiteImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);

	GetAnimator()->CreateAnimation(L"Idle_Right", m_pImg, Vec2(0.f, 128.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Idle_Left", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);

	GetAnimator()->CreateAnimation(L"Open_Right", m_pImg, Vec2(128.f, 128.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 4, false);
	GetAnimator()->CreateAnimation(L"Open_Left", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 4, false);

	GetAnimator()->CreateAnimation(L"Summon_Right", m_pImg, Vec2(640.f, 128.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 4.f, 1, false);
	GetAnimator()->CreateAnimation(L"Summon_Left", m_pImg, Vec2(640.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 4.f, 1, false);

	GetAnimator()->CreateAnimation(L"Close_Right", m_pImg, Vec2(768.f, 128.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 4, false);
	GetAnimator()->CreateAnimation(L"Close_Left", m_pImg, Vec2(768.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 4, false);

	GetAnimator()->CreateAnimation(L"Dead_Left" ,m_pImg, Vec2(1280.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Dead_Right", m_pImg, Vec2(1280.f, 128.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);


	GetAnimator()->Play(L"Idle_Right");

	CSoundManager::getInst()->AddSound(L"halfmoon_die", L"sound\\halfmoon_die.wav", false);

}

CHalfMoon::~CHalfMoon()
{
}

void CHalfMoon::update()
{
	vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	Vec2 vPlayerPos = pPlayer[0]->GetPos();
	Vec2 vPos = GetPos();
	if (GetHP())
	{
		if (false == BugLivingCheck())
		{
			if (abs(vPlayerPos.x - vPos.x) < 600.f)
			{
				isProducing = true;
			}
		}
		if (isProducing)
		{
			m_summonTimer += fDT;
		}
		if (m_summonTimer >= 2.2f
			&& m_summonTimer < 3.f)
		{
			if(isRight)
				GetAnimator()->Play(L"Open_Right");
			else
				GetAnimator()->Play(L"Open_Left");
		}
		if (m_summonTimer >= 3.f)
		{
			GenerateBug();
			if (m_summonTimer >= 3.0f
				&& m_summonTimer < 7.0f)
			if (isRight)
			{
				GetAnimator()->Play(L"Summon_Right");
			}
			else
			{
				GetAnimator()->Play(L"Summon_Left");
			}
			if (m_summonTimer >= 7.2f
				&& m_summonTimer < 7.5f)
			{
				if (isRight)
					GetAnimator()->Play(L"Close_Right");
				else
					GetAnimator()->Play(L"Close_Left");
			}
			if (m_summonTimer >= 7.5f)
			{
				if (isRight)
					GetAnimator()->Play(L"Idle_Right");
				else
					GetAnimator()->Play(L"Idle_Left");
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
				if (pBugs[i]->GetPos().x >= 0.f)
				{
					CSoundManager::getInst()->Play(L"monster_die");
					//////////////////////ÀÌÆåÆ®///////////////
					CEffect* effectDie = new CEffect(L"Effect_Die_Small", L"texture\\Animation\\Effect_Die_Small.png",
						L"Effect_Die_Small", Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 10, false, false, L"Effect_Die_Small");
					effectDie->SetPos(Vec2(pBugs[i]->GetPos()));
					effectDie->SetDuration(1.f);
					CreateObj(effectDie, GROUP_GAMEOBJ::EFFECT);
					///////////////////////////////////////////
				}
				pBugs[i]->SetPos((Vec2(-500.f, 0.f)));
				

			}
		}
	}
	else
	{
		for (int i = 0; i < pBugs.size(); ++i)
		{
			//////////////////////ÀÌÆåÆ®///////////////
			CEffect* effectDie = new CEffect(L"Effect_Die_Small", L"texture\\Animation\\Effect_Die_Small.png",
				L"Effect_Die_Small", Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 10, false, false, L"Effect_Die_Small");
			effectDie->SetPos(Vec2(pBugs[i]->GetPos()));
			effectDie->SetDuration(1.f);
			CreateObj(effectDie, GROUP_GAMEOBJ::EFFECT);
			///////////////////////////////////////////
			DeleteObj(pBugs[i]);
		}
		pBugs.clear();
	}
	
	SetPos(vPos);

	GetAnimator()->update();
}

void CHalfMoon::GenerateBug()
{
	vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	Vec2 vPlayerPos = pPlayer[0]->GetPos();
	Vec2 vPos = GetPos();
	if (vPlayerPos.x < vPos.x)
	{
		pBugs[0]->SetGoRight(false);
	}
	else
	{
		pBugs[0]->SetGoRight(true);
	}
	for (int i = 1; i < pBugs.size(); ++i)
	{
		pBugs[i]->SetGoRight(pBugs[0]->GetGoRight());
	}
	for (int i = 0; i < pBugs.size(); ++i)
	{
		if (isRight)
		{
			pBugs[i]->SetFaceRight(true);
		}
		else
		{
			pBugs[i]->SetFaceRight(false);
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
			pBugs[0]->SetState(eState_Bug::BORN);
		}
		else if (1== bugCounter)
		{
			++bugCounter;
			bugTimer = 0.f;
			pBugs[1]->SetPos(Vec2(vPos));
			pBugs[1]->SetHP(1);
			pBugs[1]->SetState(eState_Bug::BORN);
		}
		else if (2== bugCounter)
		{
			++bugCounter;
			bugTimer = 0.f;
			pBugs[2]->SetPos(Vec2(vPos));
			pBugs[2]->SetHP(1);
			pBugs[2]->SetState(eState_Bug::BORN);
		}
		else if (3== bugCounter)
		{
			++bugCounter;
			bugTimer = 0.f;
			pBugs[3]->SetPos(Vec2(vPos));
			pBugs[3]->SetHP(1);
			pBugs[3]->SetState(eState_Bug::BORN);
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
	}
	if (pOther->GetName() == L"Missile_Player")
	{
		if(isRight)
		{
			GetAnimator()->Play(L"White_Right");
		}
		else
		{
			GetAnimator()->Play(L"White_Left");
		}

		int hp = GetHP();
		SetHP(--hp);
	}
	if (GetHP() <= 0)
	{
		if (!isDead)
		{
			CSoundManager::getInst()->Play(L"halfmoon_die");
			//////////////////////ÀÌÆåÆ®///////////////
			CEffect* effectHMDie = new CEffect(L"Effect_Die_Big", L"texture\\Animation\\Effect_Die_Big.png",
				L"Effect_Die_Big", Vec2(0.f, 0.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.15f, 10, false, false, L"Effect_Die_Big");
			effectHMDie->SetPos(Vec2(GetPos()));
			effectHMDie->SetDuration(1.5f);
			CreateObj(effectHMDie, GROUP_GAMEOBJ::EFFECT);
			///////////////////////////////////////////
			isDead = true;
		}

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
			L" drctn  : " + std::to_wstring(isRight) + L"\n" +//TODO:¼öÁ¤
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