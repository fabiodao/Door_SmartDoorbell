#ifndef __CDSENSORS_H__
#define __CDSENSORS_H__

#include "CMotion.h"
#include "CButton.h"

#include <signal.h>
#include <pthread.h>
#include <mqueue.h>
#include <string>

/* max length of a message (just for this process) */
#define MAX_MSG_LEN     70

class CdSensors{
    private:
        CMotion motion;
        CButton button;

        mqd_t msgqSensors;
        pid_t pid;
        char msgcontent[MAX_MSG_LEN];

        static CdSensors *thisPtr;

        static void isrHandler(int n, siginfo_t *info, void *unused);
        void motionISR();
        void buttonISR();

    public:
        CdSensors();
        ~CdSensors();

        void run();

};


#endif