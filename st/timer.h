//class CTimer
//ms ���� timer

//windows 2000/xp ������ 5ms ������ ��Ȯ���� ������

#ifndef _timer_h_
#define _timer_h_

class CTimer
{
private:
	int m_nStartTime;

public:
	CTimer();
	~CTimer();
	void start();
	int time();

	//time �ð����κ��� interval ��ŭ�� �ð��� ��� �ߴ°�?
	bool elapsed(int &time, int interval);
};
#endif