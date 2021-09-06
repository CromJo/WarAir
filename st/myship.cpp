//myship.cpp class CMyship
#include "myship.h"

CMyShip::CMyShip()
{
}
CMyShip::~CMyShip()
{
}
void CMyShip::Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();//������ �����νð� �ʱ�ȭ
	m_nMoveInterval = MoveInterval;//������ ���� �ʱ�ȭ
}

void CMyShip::Left()
{
	if (!m_bIsLive)
	{
		return;
	}

	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{
		m_x -= 5;

		if (m_x < 30)
		{
			m_x = 30;
		}
	}
}
void CMyShip::Right()
{
	if (!m_bIsLive)
	{
		return;
	}

	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{
		m_x += 5;

		if (m_x > 770)
		{
			m_x = 770;
		}
	}
}

void CMyShip::DrawFrame(LPDIRECTDRAWSURFACE7 lpSurface, int Frame)
{
	CGObject::DrawFrame(m_x, m_y, Frame, lpSurface);
}