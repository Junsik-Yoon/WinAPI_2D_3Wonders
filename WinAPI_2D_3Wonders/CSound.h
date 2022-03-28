#pragma once
#include "FMOD/inc/fmod.hpp"
#pragma comment(lib,"FMOD/lib/fmod_vc.lib")
#include "CResource.h"

using namespace FMOD;

class CSound : public CResource
{

private:
	Sound* m_pSound;	//음악
	Channel* m_pChannel;//동시출력시 채널 
public:


	void Play(float _volume = 1.f);
	void Stop();
	void Pause();
	void Resume();

	void Load(const wstring& strFilePath);

public:
	bool IsPlay();
	bool IsPause();

	void SetLoop(bool loop);
public:
	CSound();
	~CSound();
};


