#include "CdSensors.h"
#include <cstring>
#include <iostream>

/* name of the POSIX object referencing the queue */
#define MSGQOBJ_NAME    "/msgqSensors"


CdSensors* CdSensors::thisPtr = NULL;

CdSensors::CdSensors():
    motion(isrHandler),
    button(isrHandler)
{
    msgqSensors = mq_open(MSGQOBJ_NAME, O_WRONLY | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG, NULL);
    if (msgqSensors == (mqd_t)-1) {
        mq_unlink(MSGQOBJ_NAME);
        perror("In mq_open()");
        exit(1);
    }

    thisPtr = this;
}

CdSensors::~CdSensors()
{
    mq_unlink(MSGQOBJ_NAME);
}

void CdSensors::motionISR()
{
    strcpy(msgcontent,"MV");
    mq_send(msgqSensors, msgcontent, strlen(msgcontent)+1, 1);
}

void CdSensors::buttonISR()
{
    strcpy(msgcontent,"BT");
    mq_send(msgqSensors, msgcontent, strlen(msgcontent)+1, 1);
}

void CdSensors::isrHandler(int n, siginfo_t *info, void* unused)
{
    switch(n){
        case SIGUSR1:
            thisPtr->motionISR();
            break;
        case SIGUSR2:
            thisPtr->buttonISR();
        default:
            break;
    }
}

void CdSensors::run()
{
    motion.enable();
    button.enable();
    //system("/etc/mainlocal.elf");
    while(1);
}
