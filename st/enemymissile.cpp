#include "enemymissile.h"

CEnemyMissile::CEnemyMissile()
{
}
CEnemyMissile::~CEnemyMissile()
{
}


void CEnemyMissile::Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}

void CEnemyMissile::Move()
{
	if (!m_bIsLive)
	{
		return;
	}

	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{
		m_y += 5;

		if (m_y > 600)
		{
			m_bIsLive = false;
		}
	}
}

void CEnemyMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::Draw(m_x, m_y, lpSurface);
}