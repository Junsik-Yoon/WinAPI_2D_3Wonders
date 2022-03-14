#pragma once

class CTexture;

class CCore
{
public:
	SINGLETON(CCore);
private:
	HDC		m_hDC;		//À©µµ¿ìDC
	CTexture* m_pMemTex;

	// GDI
	HBRUSH m_arrBrush[(int)TYPE_BRUSH::SIZE];
	HPEN m_arrPen[(int)TYPE_PEN::SIZE];
public:
	void init();
	void update();
	void render();

	HDC GetMainDC() { return m_hDC; }

	void CreateBrushPen();
	HBRUSH GetBrush(TYPE_BRUSH type) { return m_arrBrush[(int)type]; }
	HPEN GetPen(TYPE_PEN type) { return m_arrPen[(int)type]; }
};

