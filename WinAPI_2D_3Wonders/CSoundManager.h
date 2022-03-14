#pragma once
#include "FMOD/inc/fmod.hpp"
#pragma comment(lib,"FMOD/lib/fmod_vc.lib")

using namespace FMOD;

class CSound;

class CSoundManager
{
	SINGLETON(CSoundManager);

private:
	map<wstring, CSound*> m_mapSound; //사운드들을 관리하기 위한 자료구조
	System* m_pSystem;	//사운드들을 출력하고 관리하기 위한 코어
public:
	void init();
	void update();

	System* GetSystem() { return m_pSystem; }

	void AddSound(wstring keyName, wstring filePath, bool bgm);
	void Play(wstring keyName);
	void Stop(wstring keyName);
	void Pause(wstring keyName);
	void Resume(wstring keyName);

	bool IsPlaySound(wstring keyname);
	bool IsPauseSound(wstring keyname);
};

