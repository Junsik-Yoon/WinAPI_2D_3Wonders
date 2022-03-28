#pragma once

class CLou;
class CUI;

class CGameManager
{
	SINGLETON(CGameManager);

private:

public:
	void update();
	void init();
};

