#pragma once

class CTexture;

class CCore
{
public:
	SINGLETON(CCore);
private:
public:
	void init();
	void update();
	void render();
};

