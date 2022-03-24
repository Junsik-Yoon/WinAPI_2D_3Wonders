#pragma once

class CTexture;

class CCore
{
public:
	SINGLETON(CCore);
private:
	bool m_debugOn;
public:
	void init();
	void update();
	void render();
	void render_information();
public:
	bool DebugMode() { return m_debugOn; }
};

