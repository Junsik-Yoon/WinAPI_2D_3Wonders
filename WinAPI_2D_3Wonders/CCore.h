#pragma once

class CTexture;

class CCore
{
public:
	SINGLETON(CCore);
private:
	HDC		m_hDC;		//������DC

public:
	void init();
	void update();
	void render();

	HDC GetMainDC() { return m_hDC; }

};

