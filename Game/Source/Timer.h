#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defs.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();
	uint32 Read() const;
	float ReadSec() const;
	float ReadCombat() const;
	bool Check(float interval);

	float counter = 0.0f;

private:
	uint32 startTime;
};

#endif //__TIMER_H__