#pragma once
class CTimeManager
{
	SINGLETON(CTimeManager);

private:
	LARGE_INTEGER m_llCurCount; //현재 카운트
	LARGE_INTEGER m_llPrevCount;//이전 카운트
	LARGE_INTEGER m_llFrequency;//초당 카운트 횟수
	unsigned int m_uiFPS;		//초당 업데이트 횟수
	double m_dDT;				//업데이트와 업데이트 사이 시간
public:
	void update();
	void init();

	unsigned int	GetFPS() { return m_uiFPS; }		//1초당 프레임
	double			GetDT() { return m_dDT; }			//1프레임당 시간
	double			GetfDT() { return (float)m_dDT; }
};
