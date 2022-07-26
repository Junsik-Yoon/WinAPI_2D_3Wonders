#pragma once

class CGenerateManager
{
	SINGLETON(CGenerateManager);


private:
	float goblinTimer;
	const int goblinGenTime = 4;
	bool inited = false;
public:
	vector<CGameObject*>pGroundTile;
	vector<CGameObject*>pPlayer;

public:
	
	void GenerateGoblin();

public:
	void update();
	void init();
};

