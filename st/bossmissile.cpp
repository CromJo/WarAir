//bossmissile.cpp
//class cBossMissile
#include "bossmissile.h"

CBossMissile::CBossMissile()//생성자 : 클래스가 생성될 때 딱 한번 호출
{
	m_speedx = 2;//m_speedx에 2를 줌
}
CBossMissile::~CBossMissile()//소멸자 : 클래스가 종료될 때 딱 한번 호출
{
}

void CBossMissile::Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	//이니셜라이즈(초기화) - 객체를 만든다
	//인자값(스프라이트, x,y좌표, 시간값, 현재 프레임,프레임 간격, 움직임 간격)
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	//boss->GObject헤더에 있는 CGObject클래스 이니셜라이즈 가져옴
	m_nLastMoveTime = timer->time();
	//마지막 움직인 시간에 boss.h->gobject.h->timer.h->timer.cpp time을 가져와서
	//부팅 후 경과 시간에 게임을 시작한 시간 뺀걸 넣어줌
	m_nMoveInterval = MoveInterval;
	//움직임 간격을 보스 헤더에 있는 nMoveInterval에 넣어줌
}

void CBossMissile::Move()
{
	if (!m_bIsLive)
	{//미사일 없으면 리턴 - flase일때
		return;
	}

	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{//(쏘는 간격 조정하기위해)미사일 있을때 일정시간이 지나면
		m_x += m_speedx;//m_x에 m_speedx값만큼 더해줌
		m_y += 5; //m_y에 5더해줌

		if (m_y > 600)
		{//m_y가 600보다 클때(아래로 내려갔을때)
			m_bIsLive = false;
			//미사일 사라짐
		}
	}
}

void CBossMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{//cbossmissile draw로 미사일 그려줌
	CGObject::Draw(m_x, m_y, lpSurface);
	//CGOjbect 드로를 가져와서 미사일 없을땐 그려주지 않고 리턴, 미사일 있으면 타이머의 일랩스드 클래스로 위에 미사일 움직일때마다 그려줌
}