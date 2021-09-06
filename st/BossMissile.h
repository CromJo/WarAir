#ifndef _BossMissile_h_
#define _BossMissile_h_

#include "gobject.h"

class CBossMissile :public  CGObject
{
private:
	int m_nLastMoveTime;//������ ������ �ð�
	int m_nMoveInterval;//������ ����

public:
	int m_speedx;//x��ǥ �ӵ�

public:
	CBossMissile();//������
	~CBossMissile();//�Ҹ���
	void Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval);//�̴ϼȶ�����(�ʱ�ȭ)-��ü����
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);//�׷��ֱ�����
	void Move();//�����̱�����
};
#endif