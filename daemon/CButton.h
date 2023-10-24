#ifndef __CBUTTON_H__
#define __CBUTTON_H__

#include "CCharDevDriver.h"

#include "interrupt.h" // ISR
#include <signal.h> // sigaction

class CButton : public CCharDevDriver
{
public:
	CButton(ISR isr);
	~CButton();
	
	virtual void enable(void);
	virtual void disable(void);

private:
	struct sigaction act;
	ISR handler;
};

#endif 