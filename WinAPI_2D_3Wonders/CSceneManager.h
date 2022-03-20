#pragma once

class CScene;

class CSceneManager
{
	friend class CEventManager;

	SINGLETON(CSceneManager);
private:
	CScene* m_arrScene[(UINT)GROUP_SCENE::SIZE];
	CScene* m_pCurScene;
private:
	void ChangeScene(GROUP_SCENE group);

public:

	void update();
	void render();
	void init();

	CScene* GetCurScene() { return m_pCurScene; };
};


