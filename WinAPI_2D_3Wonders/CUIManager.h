#pragma once
class CUI;

class CUIManager
{
	SINGLETON(CUIManager);
	CUI* m_pFocusedUI;
private:

public:
	void update();
	void SetFocusedUI(CUI* pUI);//클릭 이외에도 창이 포커스를 받을 수 있도록

private:
	CUI* GetTargetUI(CUI* pParentUI);
	CUI* GetFocusedUI();

};