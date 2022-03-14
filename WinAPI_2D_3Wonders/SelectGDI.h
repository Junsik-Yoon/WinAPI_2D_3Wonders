#pragma once
class SelectGDI
{
private:
	HDC m_hDC;
	HPEN m_hDefaultPen;
	HBRUSH m_hDefaultBrush;

public:
	SelectGDI(HDC hDC, TYPE_BRUSH brushType)
	{
		m_hDC = hDC;
		HBRUSH hBrush = CCore::getInst()->GetBrush(brushType);
		m_hDefaultBrush = (HBRUSH)SelectObject(hDC, hBrush);
		m_hDefaultPen = 0;
	}
	SelectGDI(HDC hDC, TYPE_PEN penType)
	{
		m_hDC = hDC;
		HPEN hPen = CCore::getInst()->GetPen(penType);
		m_hDefaultPen = (HPEN)SelectObject(hDC, hPen);
		m_hDefaultBrush = 0;
	}
	~SelectGDI()
	{
		SelectObject(m_hDC, m_hDefaultBrush);
		SelectObject(m_hDC, m_hDefaultPen);
	}
};

