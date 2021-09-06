//exploding.cpp class CExploding
#include "exploding.h"

CExploding::CExploding()
{
}
CExploding::~CExploding()
{
}

void CExploding::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_bIsLive) return;

	if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval))
	{
		m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();
		if (m_nCurrentFrame == 0)
		{
			m_bIsLive = false;
			return;
		}
	}
	m_pSprite->Drawing(m_nCurrentFrame, m_x, m_y, lpSurface);
}