#ifndef _AIMER_HPP_
#define _AIMER_HPP_

#include "targetDetector.hpp"
#include "laserAimer.hpp"
#include "ballistics.hpp"

#include <pthread.h>
#include <iostream>


class Aimer{

public:
	Aimer(TargetDetector* detector, LaserAimer* laserAim, Ballistics* ballistics);
	virtual ~Aimer();

    void join();    

private:
	
	static void* run(void*);

	void operator()();

	TargetDetector* detector;
	LaserAimer* laserAim;
    Ballistics* eniac;

	pthread_t _this;
    bool stop;
};


#endif
