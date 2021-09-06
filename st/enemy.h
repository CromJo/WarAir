//class CEnemy
#ifndef _enemy_h
#define _enemy_h_

#include "gobject.h"

class CEnemy : public CGObject
{
private:
	int m_nLastMoveTime;//���������� ������ �ð�
	int m_nMoveInterval;//�������� �ð� ����

public:
	int m_speedx;//���� �¿������ ����
	int m_speedy;//������ �ϳ��� ���̶� �浹�Ͽ������ �Ʒ��� �̵� �ϱ� ����

public:
	CEnemy();//���� ���ǵ� �ʱ�ȭ
	~CEnemy();
	void Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval);//���� �ʱ�ȭ
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);//���⸦ �׷���
	void DrawMissile(LPDIRECTDRAWSURFACE7 lpSurface);//���̴� ���� ����(?)
	void CreateMissile();//���̴� ���� ����(?)
	void Move();//���� ������ ����
};
#endif