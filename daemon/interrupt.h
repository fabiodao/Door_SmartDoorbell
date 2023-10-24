#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <signal.h>
typedef void (*ISR)(int, siginfo_t*, void*);

#endif // !__INTERRUPT_H__