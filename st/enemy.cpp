//enemy.cpp
//class CEnemy

#include "enemy.h"

//생성자 : 클래스가 생성될 때 딱 한번 호출
CEnemy::CEnemy()
{
	//적 스피드
	m_speedx = 2;
}
//소멸자 : 클래스가 종료될 때 딱 한번 호출
CEnemy::~CEnemy()
{
}
void CEnemy::Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	//객체를 만든다
	//인자값(스프라이트, x,y좌표, 시간값, 현재 프레임,간격, 움직임 간격)
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}

void CEnemy::Move()
{
	if (!m_bIsLive)
	{//적기를 움직이는데, 만약 적기가 죽어있다면 리턴
		return;
	}

	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{//적기가 살아있을 때, 일정 시간(게임이 시작되고나서 지나간 시간을 받고, 약 1초간격으로)
		if (m_x < 50)
		{//적기가 x측으로 50보다 적으면(왼쪽 벽에 부딪힐 경우)
			//스피드를 2로 바꾸고, 아래쪽으로 한칸 내려준다.
			//스피드가 -2면 왼쪽으로 이동하고, +2면 오른쪽으로 이동한다.
			m_speedx = 2;
			m_y += 60;
		}
		if (m_x>750)
		{
			m_speedx = -2;
			m_y += 60;
		}
		m_x += m_speedx;

		if (m_y >= 500)
		{
			m_y = 60;
		}
	}
}

void CEnemy::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::Draw(m_x, m_y, lpSurface);
}