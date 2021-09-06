// mymissile.cpp
// class CMyMissile

#include "mymissile.h"

CMyMissile::CMyMissile()
{
}
CMyMissile::~CMyMissile()
{
}


void CMyMissile::Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}

void CMyMissile::Move()
{
	if(!m_bIsLive)
	{
		return;
	}

	if(m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{
		m_y -= 8;

		if(m_y < -100)
		{
			m_bIsLive = false;
		}
	}
}

void CMyMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::Draw(m_x,m_y,lpSurface);
}