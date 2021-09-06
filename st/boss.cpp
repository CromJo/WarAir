//boss.cpp
//class CBoss

#include "boss.h"//보스헤더 가져옴 - 전처리기 지시문 인클루드로 보스헤더를 포함하고
//생성자 : 클래스가 생성될 때 딱 한번 호출
CBoss::CBoss()
{ //보스 스피드 2
	m_speedx = 2;
}
//소멸자 : 클래스가 종료될 때 딱 한번 호출
CBoss::~CBoss()
{
}

void CBoss::Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	//이니셜라이즈(초기화) - 객체를 만들기 위해/객체초기화
	//인자값(스프라이트, x,y좌표, 시간값, 현재 프레임,프레임 간격, 움직임 간격)
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	//boss->GObject헤더에 있는 CGObject클래스 범위지정연산자로 이니셜라이즈 호출
	m_nLastMoveTime = timer->time();
	//마지막 움직인 시간에 boss.h->gobject.h->timer.h->timer.cpp time을 가져와서
	//부팅 후 경과 시간에 게임을 시작한 시간 뺀걸 넣어줌
	m_nMoveInterval = MoveInterval;
	//움직임 간격을 보스 헤더에 있는 nMoveInterval에 넣어줌
	m_myx = m_x;
	//보스 헤더에있는 myx에 gobjbect헤더에 있는 m_x를 넣어줌 - 근대 만들어두고 쓰이는대가 없음

}

void CBoss::Move()
{
		if (!m_bIsLive)//m_blsLive가 false
		{//보스를 움직이는데, 만약 보스가 죽어있다면 리턴
			return;
		}
		//포인터 멤버 연산자
		if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
		{//보스가 살아있을 때, timer의 일랩스드 클래스 가져와서 라스트무브타임값에 무브인터벌만큼 지나면-즉, 일정시간 지나면
			//if(m_x<50){m_speedx=2;} // 현재 주석처리 되있음 - m_x가 50보다 작으면 speedx=2
			//if(m_x>750){m_speedx=-2;} //m_x가 750보다 크면 speedx=-2
			//m_x+=m_speedx; //m_x에 speedx만큼 플러스 - 좌우로 왔다갔다 하게하려다 지운듯
		}
	
}
void CBoss::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{//보스를 그려줌
	CGObject::Draw(m_x, m_y, lpSurface);
	//CGOjbect 드로 호출. 죽었을경우 그려주지 않고 리턴, 살아있으면 위에 한것처럼 타이머의 일랩스드 클래스로 일정시간 지날때마다 그려주는데 즉, boss에 move가 실행될때마다 그려줌
}