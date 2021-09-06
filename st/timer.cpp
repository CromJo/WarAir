#include <windows.h> //윈도우 기본 구조체와 함수

#include "timer.h"

CTimer::CTimer()//생성자
{
	m_nStartTime = 0;//처음 시작 시간을 0으로 초기화
}

CTimer::~CTimer()
{
}

void CTimer::start()//윈도우 생성시 호출
{
	m_nStartTime = timeGetTime(); //timeGetTime()은 win32 api 함수
	//결과 값은 부팅 후 경과 시간(ms)
}

int CTimer::time()
{
	return (timeGetTime() - m_nStartTime);//부팅 후 경과 시간 - 게임을 시작한 시간
}

bool CTimer::elapsed(int &stime, int interval)
{
	int current_time = time();
	if (current_time >= stime + interval)//마지막으로 해당 장면을 실행한지(ex-미사일을 쏜지)딜레이 간격(interval)만큼 지나면
		//#define MY_MISSILE_INTERVAL 240
	{
		stime = current_time;//현재 시간을 미사일을 쏠 시간으로 변경
		return true;//미사일을 쏠수 있게 true
	}
	else//간격만큼 안지나면
		return false;//미사일을 못 쏘게 될경우 false
}