#pragma once

class CLou;
class CUI;
class CStatusUI;
class CScoreUI;

class CGameManager
{
	SINGLETON(CGameManager);


private:
	bool g_isRendering;
	CLou* g_pPlayer;
	int g_gameScore;
	int g_playerLife;
	int g_heartsHave;
	wstring g_missiletype;

public:
	void	SetRender(bool _render) { g_isRendering = _render; }
	bool	GetRender() { return g_isRendering; }
	void	SetPlayer(CLou* _lou) { g_pPlayer = _lou; }
	CLou*	GetPlayer() { return g_pPlayer; }
	void	SetGameScore(int _score) { g_gameScore = _score; }
	int		GetGameScore() { return g_gameScore; }
	void	SetPlayerLife(int _life) { g_playerLife = _life; }
	int		GetPlayerLife() { return g_playerLife; }
	void	SetHeart(int _heart) { g_heartsHave = _heart; }
	int		GetHeart() { return g_heartsHave; }
	void	SetMissileType(const wstring &_missileType) { g_missiletype = _missileType; }
	const wstring& GetMissileType() { return g_missiletype; }
	
public:
	void update();
	void init();
};

