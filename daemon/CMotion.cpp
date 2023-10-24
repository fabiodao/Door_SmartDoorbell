#include "CMotion.h"


#define SIGH SIGUSR1
#define DEV_NAME "motion"

CMotion::CMotion(ISR isr) : CCharDevDriver(DEV_NAME)
{
	sigemptyset(&act.sa_mask);
	handler = isr; 
}

CMotion::~CMotion()
{
	CCharDevDriver::Close();
}

void CMotion::enable(void)
{	
	CCharDevDriver::Open();

	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handler;
	
	sigaction(SIGUSR1, &act, NULL);
}

void CMotion::disable(void)
{
	CCharDevDriver::Close();

	act.sa_handler = SIG_IGN;

	sigaction(SIGUSR1, &act, NULL);
}
