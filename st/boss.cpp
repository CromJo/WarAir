//boss.cpp
//class CBoss

#include "boss.h"//������� ������ - ��ó���� ���ù� ��Ŭ���� ��������� �����ϰ�
//������ : Ŭ������ ������ �� �� �ѹ� ȣ��
CBoss::CBoss()
{ //���� ���ǵ� 2
	m_speedx = 2;
}
//�Ҹ��� : Ŭ������ ����� �� �� �ѹ� ȣ��
CBoss::~CBoss()
{
}

void CBoss::Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval)
{
	//�̴ϼȶ�����(�ʱ�ȭ) - ��ü�� ����� ����/��ü�ʱ�ȭ
	//���ڰ�(��������Ʈ, x,y��ǥ, �ð���, ���� ������,������ ����, ������ ����)
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval);
	//boss->GObject����� �ִ� CGObjectŬ���� �������������ڷ� �̴ϼȶ����� ȣ��
	m_nLastMoveTime = timer->time();
	//������ ������ �ð��� boss.h->gobject.h->timer.h->timer.cpp time�� �����ͼ�
	//���� �� ��� �ð��� ������ ������ �ð� ���� �־���
	m_nMoveInterval = MoveInterval;
	//������ ������ ���� ����� �ִ� nMoveInterval�� �־���
	m_myx = m_x;
	//���� ������ִ� myx�� gobjbect����� �ִ� m_x�� �־��� - �ٴ� �����ΰ� ���̴´밡 ����

}

void CBoss::Move()
{
		if (!m_bIsLive)//m_blsLive�� false
		{//������ �����̴µ�, ���� ������ �׾��ִٸ� ����
			return;
		}
		//������ ��� ������
		if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
		{//������ ������� ��, timer�� �Ϸ����� Ŭ���� �����ͼ� ��Ʈ����Ÿ�Ӱ��� �������͹���ŭ ������-��, �����ð� ������
			//if(m_x<50){m_speedx=2;} // ���� �ּ�ó�� ������ - m_x�� 50���� ������ speedx=2
			//if(m_x>750){m_speedx=-2;} //m_x�� 750���� ũ�� speedx=-2
			//m_x+=m_speedx; //m_x�� speedx��ŭ �÷��� - �¿�� �Դٰ��� �ϰ��Ϸ��� �����
		}
	
}
void CBoss::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{//������ �׷���
	CGObject::Draw(m_x, m_y, lpSurface);
	//CGOjbect ��� ȣ��. �׾������ �׷����� �ʰ� ����, ��������� ���� �Ѱ�ó�� Ÿ�̸��� �Ϸ����� Ŭ������ �����ð� ���������� �׷��ִµ� ��, boss�� move�� ����ɶ����� �׷���
}