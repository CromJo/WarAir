//class CBoss
#ifndef _Boss_h_
#define _Boss_h_
//헤더가 여러개의 파일에 포함될경우 중복정의 문제 발생하게 되는데, #ifndef와 #define지시자를 사용해 중복체크

#include "gobject.h"

class CBoss :public CGObject
{
private:
	int m_nLastMoveTime; //마지막 움직인 시간
	int m_nMoveInterval; //움직임 간격

public:
	int m_myx; //boss 이니셜라이즈에서 m_x값을 넣어주긴 하는데 쓰이는 곳이 없음
	int m_speedx; //x좌표 속도
	int m_speedy; //y좌표 속도
	int m_Enegy; //에너지

public:
	CBoss();//생성자
	~CBoss();//소멸자
	void Initialize(CSprite *pSprite, int x, int y, CTimer *time, int CurrentFrame, int FrameInterval, int MoveInterval);//이니셜라이즈(초기화)-객체생성
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);//보스 그려줌
	void DrawMissile(LPDIRECTDRAWSURFACE7 lpSurface);//미사일 그려줌
	void CreateMissile();//미사일 만듬
	void Move();//움직임
};
#endif //if지시문으로 시작한 조건부 지시문은 endif 지시문으로 종료해야됨