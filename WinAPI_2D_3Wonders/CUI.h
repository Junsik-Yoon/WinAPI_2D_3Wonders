#pragma once
#include "CGameObject.h"
class CUI : public CGameObject
{
	friend class CUIManager;
private:
	vector<CUI*> m_vecChildUI;
	CUI* m_pParentUI;

	Vec2 m_fptFinalPos; //상대적인 자신이 그려질 위치(게임안이나 부모ui안에 있을 때)


	bool m_bCameraAffected; //카메라 위치를 ui가 영향을 받는가에대한 변수
	bool m_bMouseOn;	//UI가 이전 프레임에 마우스가 올려져 있었다.
	bool m_bLbtnDown;	//UI가 이전 프레임에 눌렸었다.

public:
	virtual void update();
	virtual void finalupdate();
	virtual void render();

	void update_child();
	void finalupdate_child();
	void render_child();

	virtual void MouseOn();
	virtual void MouseLbtnDown();//클릭만으로는 그자리에서 클릭하고 클릭해제하는것밖에 감지할수없음
	virtual void MouseLbtnUp();//아니면 눌림처리에서 버튼 이펙트가 바뀐다던가 할 때 사용함
	virtual void MouseLbtnClicked();

public:
	Vec2 GetFinalPos() { return m_fptFinalPos; }
	CUI* GetParent() { return m_pParentUI; }

	void AddChild(CUI* pUI);
	const vector<CUI*>& GetChildUI();//모든자식반환

	void SetCameraAffected(bool affected)
	{
		m_bCameraAffected = affected;
		for (int i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->m_bCameraAffected = affected;
		}
	}
	bool GetCameraAffected() { return m_bCameraAffected; }
	bool IsMouseOn() { return m_bMouseOn; }
	bool IsLbtnDown() { return m_bLbtnDown; }



private:
	void MouseOnCheck();

public:
	CUI();
	CUI(const CUI& other);
	virtual ~CUI();
	virtual CUI* Clone()
	{
		CUI* newUI = new CUI(*this);
		newUI->m_vecChildUI.clear();
		for (UINT i = 0; i < m_vecChildUI.size(); ++i)
		{
			newUI->m_vecChildUI.push_back(new CUI(*m_vecChildUI[i]));
			newUI->m_vecChildUI[i]->m_pParentUI = newUI;//부모세팅
		}
		return newUI;
	}
};

