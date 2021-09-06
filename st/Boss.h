//class CBoss
#ifndef _Boss_h_
#define _Boss_h_
//����� �������� ���Ͽ� ���Եɰ�� �ߺ����� ���� �߻��ϰ� �Ǵµ�, #ifndef�� #define�����ڸ� ����� �ߺ�üũ

#include "gobject.h"

class CBoss :public CGObject
{
private:
	int m_nLastMoveTime; //������ ������ �ð�
	int m_nMoveInterval; //������ ����

public:
	int m_myx; //boss �̴ϼȶ������ m_x���� �־��ֱ� �ϴµ� ���̴� ���� ����
	int m_speedx; //x��ǥ �ӵ�
	int m_speedy; //y��ǥ �ӵ�
	int m_Enegy; //������

public:
	CBoss();//������
	~CBoss();//�Ҹ���
	void Initialize(CSprite *pSprite, int x, int y, CTimer *time, int CurrentFrame, int FrameInterval, int MoveInterval);//�̴ϼȶ�����(�ʱ�ȭ)-��ü����
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);//���� �׷���
	void DrawMissile(LPDIRECTDRAWSURFACE7 lpSurface);//�̻��� �׷���
	void CreateMissile();//�̻��� ����
	void Move();//������
};
#endif //if���ù����� ������ ���Ǻ� ���ù��� endif ���ù����� �����ؾߵ�