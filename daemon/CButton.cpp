#include "CButton.h"

#define DEV_NAME "button"

CButton::CButton(ISR isr) : CCharDevDriver(DEV_NAME)
{
	sigemptyset(&act.sa_mask);
	handler = isr; 
}

CButton::~CButton()
{
	CCharDevDriver::Close();
}

void CButton::enable(void)
{	
	CCharDevDriver::Open();

	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handler;
	
	sigaction(SIGUSR2, &act, NULL);
}

void CButton::disable(void)
{
	CCharDevDriver::Close();

	act.sa_handler = SIG_IGN;

	sigaction(SIGUSR2, &act, NULL);
}