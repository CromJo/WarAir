//bossmissile.cpp
//class cBossMissile
#include "bossmissile.h"

CBossMissile::CBossMissile()//������ : Ŭ������ ������ �� �� �ѹ� ȣ��
{
	m_speedx = 2;//m_speedx�� 2�� ��
}
CBossMissile::~CBossMissile()//�Ҹ��� : Ŭ������ ����� �� �� �ѹ� ȣ��
{
}

void CBossMissile::Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	//�̴ϼȶ�����(�ʱ�ȭ) - ��ü�� �����
	//���ڰ�(��������Ʈ, x,y��ǥ, �ð���, ���� ������,������ ����, ������ ����)
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	//boss->GObject����� �ִ� CGObjectŬ���� �̴ϼȶ����� ������
	m_nLastMoveTime = timer->time();
	//������ ������ �ð��� boss.h->gobject.h->timer.h->timer.cpp time�� �����ͼ�
	//���� �� ��� �ð��� ������ ������ �ð� ���� �־���
	m_nMoveInterval = MoveInterval;
	//������ ������ ���� ����� �ִ� nMoveInterval�� �־���
}

void CBossMissile::Move()
{
	if (!m_bIsLive)
	{//�̻��� ������ ���� - flase�϶�
		return;
	}

	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
	{//(��� ���� �����ϱ�����)�̻��� ������ �����ð��� ������
		m_x += m_speedx;//m_x�� m_speedx����ŭ ������
		m_y += 5; //m_y�� 5������

		if (m_y > 600)
		{//m_y�� 600���� Ŭ��(�Ʒ��� ����������)
			m_bIsLive = false;
			//�̻��� �����
		}
	}
}

void CBossMissile::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{//cbossmissile draw�� �̻��� �׷���
	CGObject::Draw(m_x, m_y, lpSurface);
	//CGOjbect ��θ� �����ͼ� �̻��� ������ �׷����� �ʰ� ����, �̻��� ������ Ÿ�̸��� �Ϸ����� Ŭ������ ���� �̻��� �����϶����� �׷���
}