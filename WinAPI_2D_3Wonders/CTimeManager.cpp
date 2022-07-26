#include "pch.h"
#include "CTimeManager.h"

CTimeManager::CTimeManager()
{
	m_uiFPS = 0;
	m_dDT = 0;
	m_llPrevCount = {};
	m_llCurCount = {};
	m_llFrequency = {};
}
CTimeManager::~CTimeManager()
{
}

void CTimeManager::update()
{
	static unsigned int updateCount = 0;
	static double updateOneSecond = 0;
	QueryPerformanceCounter(&m_llCurCount);
	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / m_llFrequency.QuadPart;
	m_llPrevCount = m_llCurCount;

	//1초에 몇 번 업데이트가 되었는지
	++updateCount;
	updateOneSecond += m_dDT;

	//if (m_dDT > 0.1)//DT가 디버그모드 디포커스상태에서 일정 이상을 못넘기도록
	//{
	//	m_dDT = 0.1;
	//}

	if (updateOneSecond >= 1.f)
	{
		m_uiFPS = updateCount;
		updateOneSecond = 0;
		updateCount = 0;
	}
}


void CTimeManager::init()
{
	QueryPerformanceCounter(&m_llPrevCount);	//현재 시간의 카운트 수
	QueryPerformanceFrequency(&m_llFrequency);	//1초당 진행하는 카운트 수
}


