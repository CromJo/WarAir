//enemy.cpp
//class CEnemy

#include "enemy.h"

//������ : Ŭ������ ������ �� �� �ѹ� ȣ��
CEnemy::CEnemy()
{
	//�� ���ǵ�
	m_speedx = 2;
}
//�Ҹ��� : Ŭ������ ����� �� �� �ѹ� ȣ��
CEnemy::~CEnemy()
{
}
void CEnemy::Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	//��ü�� �����
	//���ڰ�(��������Ʈ, x,y��ǥ, �ð���, ���� ������,����, ������ ����)
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	m_nLastMoveTime = timer->time();
	m_nMoveInterval = MoveInterval;
}

void CEnemy::Move()
{
	if (!m_bIsLive)
	{//���⸦ �����̴µ�, ���� ���Ⱑ �׾��ִٸ� ����
		return;
	}

	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{//���Ⱑ ������� ��, ���� �ð�(������ ���۵ǰ��� ������ �ð��� �ް�, �� 1�ʰ�������)
		if (m_x < 50)
		{//���Ⱑ x������ 50���� ������(���� ���� �ε��� ���)
			//���ǵ带 2�� �ٲٰ�, �Ʒ������� ��ĭ �����ش�.
			//���ǵ尡 -2�� �������� �̵��ϰ�, +2�� ���������� �̵��Ѵ�.
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