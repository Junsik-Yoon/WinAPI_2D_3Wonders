#pragma once

class CTexture;

class CCore
{
public:
	SINGLETON(CCore);
private:
	HDC		m_hDC;		//À©µµ¿ìDC

public:
	void init();
	void update();
	void render();

	HDC GetMainDC() { return m_hDC; }

};

