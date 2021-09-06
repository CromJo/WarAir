//GObject.cpp
//class CGObject

#include "GObject.h"
//#include "define2.h"

CGObject::CGObject()
{
	m_nCurrentFrame = 0;//������ �ʱ�ȭ
	m_bIsLive = false;//��� ��ü ���� ���� �ʱ�ȭ
}

CGObject::~CGObject()
{
}

void CGObject::Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval)
{
	m_pSprite = pSprite; //��������Ʈ
	m_nCurrentFrame = CurrentFrame;//������
	m_x = x; //x��ǥ
	m_y = y; //y��ǥ
	m_nFrameInterval = FrameInterval; //������ ����
	m_bIsLive = true; //���� ����(�������)
	m_pTimer = timer; //Ÿ�̸�
	m_nLastFrameTime = m_pTimer->time();//�������� �������� �׷��� ���
}

bool CGObject::IsLive()//��� ��ü ���� ���� Ȯ���� ��� ȣ���ϴ� �Լ�
{
	return m_bIsLive;//���� ���θ� �������ش�.
}

void CGObject::Kill()//�׾��� ��� ȣ��Ǵ� �Լ�
{
	m_bIsLive = false;//��ü�� ����
}

void CGObject::Draw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_bIsLive)//bIsLive�� false�� ���
	{
		return; //�׷����� �ʰ� �׳� ����
	}
	if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval))//�����ʰ�(blsLive true), ���� ���ݸ���
	{
		m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();//drawing���� ���(������ ���Ҷ����� �Լ� �׷��ֱ�����)

	}
	m_pSprite->Drawing(m_nCurrentFrame, m_x, m_y, lpSurface);//��������Ʈ drawingȣ��
}
void CGObject::DrawFrame(int x, int y, int nFrame, LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_bIsLive)//��ü�� �׾��� ���
	{
		return;//�׷����� �ʰ� �׳� ����
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

int CGObject::GetX()//x��ǥ �Լ�
{
	return m_x;//x��ǥ ����
}

int CGObject::GetY()//y��ǥ �Լ�
{
	return m_y;//y��ǥ �Լ�
}