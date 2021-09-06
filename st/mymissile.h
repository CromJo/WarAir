//class CMyMissile
#ifndef _mymissile_h_
#define _mymissile_h_

#include "gobject.h"

class CMyMissile :public CGObject
{
private:
	int m_nLastMoveTime;//���������� ������ �ð�
	int m_nMoveInterval;//������ �ð� ����

public:
	CMyMissile();
	~CMyMissile();
	void Initialize(CSprite *pSprite, int, int y, CTimer *timer, int CurrentFrame, int FrameInterval, int MoveInterval);//�÷��̾� �̻��� �ʱ�ȭ
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);//�÷��̾� �̻��� �׷���
	void Move(); //�÷��̾� �̻��� ������ ����
};
#endif