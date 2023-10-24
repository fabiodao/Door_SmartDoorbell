#ifndef __CLOCALS_H__
#define __CLOCALS_H__

#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <mqueue.h>
#include <sched.h>
#include "CCamera.h"
#include "CLivestream.h"
#include "CDatabase.h"

using namespace std;


class CLocalS{

private:
    CCamera camera;
    CLivestream livestream;
    CDatabase database;

    pthread_t tUpdateFlags_id;
	pthread_t tRecvSensors_id;
	pthread_t tFaceDetection_id;
    pthread_t tLivestream_id;
    pthread_t tLivestreamAudio_id;

	pthread_mutex_t mutCamFrame;
    pthread_mutex_t mutStreamFlag;

    pthread_cond_t condCamFrame; 
    pthread_cond_t condStreamFlag;
    pthread_cond_t condAudioFlag;
    


    mqd_t msgqSensors;

    int livestream_status;
    int notification_flag;

    static void sigHandler(int sig);
    static void* tRecvSensors(void*);
    static void* tUpdateFlags(void*);
    static void* tLivestream(void*);
    static void* tFaceDetection(void*);
    static void* tLivestreamAudio(void*);

public:
    CLocalS();
    ~CLocalS();

    void run();



};

#endif

