//class CTimer
//ms 단위 timer

//windows 2000/xp 에서는 5ms 정도의 정확도를 가진다

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

	//time 시각으로부터 interval 만큼의 시간이 경과 했는가?
	bool elapsed(int &time, int interval);
};
#endif