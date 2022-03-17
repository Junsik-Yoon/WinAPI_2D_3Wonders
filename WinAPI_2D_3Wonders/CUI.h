#pragma once
#include "CGameObject.h"
class CUI : public CGameObject
{
	friend class CUIManager;
private:
	vector<CUI*> m_vecChildUI;
	CUI* m_pParentUI;

	Vec2 m_fptFinalPos; //������� �ڽ��� �׷��� ��ġ(���Ӿ��̳� �θ�ui�ȿ� ���� ��)


	bool m_bCameraAffected; //ī�޶� ��ġ�� ui�� ������ �޴°������� ����
	bool m_bMouseOn;	//UI�� ���� �����ӿ� ���콺�� �÷��� �־���.
	bool m_bLbtnDown;	//UI�� ���� �����ӿ� ���Ⱦ���.

public:
	virtual void update();
	virtual void finalupdate();
	virtual void render();

	void update_child();
	void finalupdate_child();
	void render_child();

	virtual void MouseOn();
	virtual void MouseLbtnDown();//Ŭ�������δ� ���ڸ����� Ŭ���ϰ� Ŭ�������ϴ°͹ۿ� �����Ҽ�����
	virtual void MouseLbtnUp();//�ƴϸ� ����ó������ ��ư ����Ʈ�� �ٲ�ٴ��� �� �� �����
	virtual void MouseLbtnClicked();

public:
	Vec2 GetFinalPos() { return m_fptFinalPos; }
	CUI* GetParent() { return m_pParentUI; }

	void AddChild(CUI* pUI);
	const vector<CUI*>& GetChildUI();//����ڽĹ�ȯ

	bool GetCameraAffected() { return m_bCameraAffected; }
	bool IsMouseOn() { return m_bMouseOn; }
	bool IsLbtnDown() { return m_bLbtnDown; }



private:
	void MouseOnCheck();

public:
	CUI(bool bCameraAffected);
	CUI(const CUI& other);
	virtual ~CUI();
	virtual CUI* Clone() = 0;
};

