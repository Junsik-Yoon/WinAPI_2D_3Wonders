#include "pch.h"
#include "CPanelUI.h"

CPanelUI::CPanelUI()
	: CUI(false)
{
	m_fptDragStart = {};
}

CPanelUI::~CPanelUI()
{
}

CPanelUI* CPanelUI::Clone()
{
	return new CPanelUI(*this);
}

void CPanelUI::MouseOn()
{
	if (IsLbtnDown())
	{
		Vec2 fptDiff = MousePos() - m_fptDragStart;

		Vec2 fptCurPos = GetPos();
		fptCurPos += fptDiff;
		SetPos(fptCurPos);

		m_fptDragStart = MousePos();
	}
}

void CPanelUI::MouseLbtnDown()
{
	m_fptDragStart = MousePos();
}

void CPanelUI::MouseLbtnUp()
{
}