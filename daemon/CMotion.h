#ifndef __CMOTION_H__
#define __CMOTION_H__

#include "CCharDevDriver.h"

#include "interrupt.h" // ISR
#include <signal.h> // sigaction

class CMotion : public CCharDevDriver
{
public:
	CMotion(ISR isr);
	~CMotion();
	
	virtual void enable(void);
	virtual void disable(void);

private:
	struct sigaction act;
	ISR handler;
};

#endif 