#pragma once
#include "CUI.h"
class CPanelUI : public CUI
{
private:
	Vec2 m_fptDragStart;

public:
	CPanelUI();
	~CPanelUI();

	virtual CPanelUI* Clone();

	virtual void MouseOn();
	virtual void MouseLbtnDown();
	virtual void MouseLbtnUp();
};
