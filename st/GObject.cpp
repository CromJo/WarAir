//GObject.cpp
//class CGObject

#include "GObject.h"
//#include "define2.h"

CGObject::CGObject()
{
	m_nCurrentFrame = 0;//프레임 초기화
	m_bIsLive = false;//모든 기체 생사 여부 초기화
}

CGObject::~CGObject()
{
}

void CGObject::Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval)
{
	m_pSprite = pSprite; //스프라이트
	m_nCurrentFrame = CurrentFrame;//프레임
	m_x = x; //x좌표
	m_y = y; //y좌표
	m_nFrameInterval = FrameInterval; //프레임 간격
	m_bIsLive = true; //생사 여부(살아있음)
	m_pTimer = timer; //타이머
	m_nLastFrameTime = m_pTimer->time();//프레임이 마지막에 그려진 사건
}

bool CGObject::IsLive()//모든 기체 생사 여부 확인할 경우 호출하는 함수
{
	return m_bIsLive;//생사 여부를 리턴해준다.
}

void CGObject::Kill()//죽었을 경우 호출되는 함수
{
	m_bIsLive = false;//기체가 죽음
}

void CGObject::Draw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_bIsLive)//bIsLive가 false일 경우
	{
		return; //그려주지 않고 그냥 리턴
	}
	if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval))//죽지않고(blsLive true), 일정 간격마다
	{
		m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();//drawing에서 사용(프레임 변할때마다 게속 그려주기위해)

	}
	m_pSprite->Drawing(m_nCurrentFrame, m_x, m_y, lpSurface);//스프라이트 drawing호출
}
void CGObject::DrawFrame(int x, int y, int nFrame, LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_bIsLive)//기체가 죽었을 경우
	{
		return;//그려주지 않고 그냥 리턴
	}
	if (nFrame >= m_pSprite->GetNumberOfFrame())
	{
		nFrame = m_pSprite->GetNumberOfFrame() - 1;
	}
	if (nFrame < 0)
	{
		nFrame = 0;
	}

	m_pSprite->Drawing(nFrame, m_x, m_y, lpSurface);
}

int CGObject::GetX()//x좌표 함수
{
	return m_x;//x좌표 리턴
}

int CGObject::GetY()//y좌표 함수
{
	return m_y;//y좌표 함수
}