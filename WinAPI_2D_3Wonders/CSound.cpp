#include "pch.h"
#include "CSound.h"


CSound::CSound()
{
	m_pSound = nullptr;
	m_pChannel = nullptr;
}

CSound::~CSound()
{
	m_pSound->release();
}


void CSound::Play()
{
	CSoundManager::getInst()->GetSystem()->playSound(m_pSound, 0, false, &m_pChannel);
	assert(m_pChannel);
}

void CSound::Stop()
{
	if (nullptr != m_pChannel)
		m_pChannel->stop();
}

void CSound::Pause()
{
	if (nullptr != m_pChannel)
		m_pChannel->setPaused(true);
}

void CSound::Resume()
{
	if (nullptr != m_pChannel)
		m_pChannel->setPaused(false);
}

void CSound::Load(const wstring& strFilePath)
{
	char str[255];
	wcstombs_s(nullptr, str, strFilePath.c_str(), 255);
	CSoundManager::getInst()->GetSystem()->createSound(str, FMOD_DEFAULT, nullptr, &m_pSound);
}

bool CSound::IsPlay()
{
	bool paused;
	if (nullptr != m_pChannel)
	{
		m_pChannel->getPaused(&paused);
		return !paused;
	}
	assert(nullptr);
}

bool CSound::IsPause()
{
	bool paused;
	if (nullptr != m_pChannel)
	{
		m_pChannel->getPaused(&paused);
		return paused;
	}
	assert(nullptr);
}

void CSound::SetLoop(bool loop)
{
	if (loop)
	{
		m_pSound->setMode(FMOD_LOOP_NORMAL);
	}
	else
	{
		m_pSound->setMode(FMOD_LOOP_OFF);
	}
}
