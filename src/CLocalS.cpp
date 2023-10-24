#define DEBUG

#include "../inc/CLocalS.h"
#include "../inc/utils.h"
#include <mqueue.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include "debug.h"


/* name of the POSIX object referencing the queue */
#define MSGQ_NAME    "/msgqSensors"
/* max length of a message (just for this process) */
#define MAX_MSG_LEN     10000

CLocalS :: CLocalS() :

    livestream(),
	camera(),
	database("product1")
	
{

		DEBUG_MSG("Cria threads");
    livestream_status = 0;
	notification_flag = 1;

	if(pthread_mutex_init(&mutCamFrame, NULL)!=0) panic("CLocalS(): Mutex init");
    if(pthread_mutex_init(&mutStreamFlag, NULL)!=0) panic("CLocalS(): Mutex init");


	if(pthread_cond_init(&condCamFrame, NULL)!=0) panic("CLocalS(): Condition variable init");
    if(pthread_cond_init(&condStreamFlag, NULL)!=0) panic("CLocalS(): Condition variable init");
	if(pthread_cond_init(&condAudioFlag, NULL)!=0) panic("CLocalS(): Condition variable init");

	msgqSensors = mq_open(MSGQ_NAME, O_RDONLY, NULL);
	if (msgqSensors == (mqd_t)-1) {
        panic("CLocalS(): Opening message queue");
    }
    
    

	if(pthread_create(&tUpdateFlags_id, NULL, tUpdateFlags, this)!=0) panic("CLocalS(): pthread create");
	if(pthread_create(&tRecvSensors_id, NULL, tRecvSensors, this)!=0) panic("CLocalS(): pthread create");
	if(pthread_create(&tFaceDetection_id, NULL, tFaceDetection, this)!=0) panic("CLocalS(): pthread create");
    if(pthread_create(&tLivestream_id, NULL, tLivestream, this)!=0) panic("CLocalS(): pthread create");
	if(pthread_create(&tLivestreamAudio_id, NULL, tLivestreamAudio, this)!=0) panic("CLocalS(): pthread create");


}

CLocalS :: ~CLocalS () {}

void CLocalS :: run(){
	
	signal(SIGINT, sigHandler);
    pthread_join(tFaceDetection_id, NULL);
    pthread_join(tLivestream_id, NULL);
    pthread_join(tUpdateFlags_id, NULL);
    pthread_join(tRecvSensors_id, NULL);
    pthread_join(tLivestreamAudio, NULL);

}




void* CLocalS :: tRecvSensors(void* arg){

	CLocalS *obj = reinterpret_cast<CLocalS*>(arg);

	char msg[MAX_MSG_LEN];
	int ret;
 

		DEBUG_MSG("Recebe sensores");	
 
	while(obj)
	{
	

		ret = mq_receive(obj->msgqSensors, msg, MAX_MSG_LEN, NULL);
				
 		if(ret == -1)
        {
            panic("tRecvSensors(): message queue receive");
		}
		else
		{
			
			if((strcmp(msg,"MV")==0) && (obj->livestream_status == 0)){
				pthread_cond_signal(&obj->condCamFrame);
				cout << "Movimento" << endl; 
				}
				
			else if((strcmp(msg,"BT")==0) && (obj->livestream_status == 0)){
				pthread_mutex_lock(&obj->mutStreamFlag);
				obj->notification_flag = - obj->notification_flag;
				obj->database.push_data("notification_flag", obj->notification_flag);
				pthread_mutex_unlock(&obj->mutStreamFlag);
				cout << "Button Pressed" << endl;
				pid_t pid = fork();
				if (pid == 0) {
					// Child process
					char *args[] = {"omxplayer", "doorbell_ring.wav", NULL};
					execvp("omxplayer", args);    
				    } else {
					// Parent process
					int status;
					waitpid(pid, &status, WNOHANG);
				 }
			}
		}

	}
	return NULL;
}

void* CLocalS :: tUpdateFlags(void *arg){

	
		DEBUG_MSG("Atuliza as flags");

	CLocalS *obj = reinterpret_cast<CLocalS*>(arg);
	
	int stream_flag = 0;
	int previous_stream_flag = 0;
	int audio_flag = 0;
	
	while(obj){
		
		sleep(1);
		
		pthread_mutex_lock(&obj->mutStreamFlag);
		
		stream_flag = obj->database.get_data("stream flag");

		audio_flag = obj->database.get_data("new_audio");

		if(stream_flag != previous_stream_flag)
			pthread_cond_signal(&obj->condStreamFlag);

		if(audio_flag == 1 && obj->livestream_status == 1)
			pthread_cond_signal(&obj->condAudioFlag);
		
		previous_stream_flag = stream_flag;

		pthread_mutex_unlock(&obj->mutStreamFlag);
	}
	return NULL;
}

void* CLocalS :: tLivestream(void *arg){

	CLocalS *obj = reinterpret_cast<CLocalS*>(arg);

	while(obj){
		
		pthread_mutex_lock(&obj->mutStreamFlag);
		
		pthread_cond_wait(&obj->condStreamFlag, &obj->mutStreamFlag);

		obj->livestream.start();
        obj->livestream_status = 1;
				
		pthread_cond_wait(&obj->condStreamFlag, &obj->mutStreamFlag);

		obj->livestream.stop();
        obj->livestream_status = 0;

		pthread_mutex_unlock(&obj->mutStreamFlag);
	}

	return NULL;
}

void* CLocalS :: tFaceDetection(void * arg){

	CLocalS *obj = reinterpret_cast<CLocalS*>(arg);

	bool face;
	
	string video_file;

	while(obj){
		
		pthread_mutex_lock(&obj->mutCamFrame);
		
		pthread_cond_wait(&obj->condCamFrame, &obj->mutCamFrame);

		face = obj->camera.detectface();
		
		cout << "Tenta detetar cara" << endl;

		if(face == true)
		{
			cout << "Cara detetada"<< endl;
			
			video_file = obj->camera.record();
			
			pthread_mutex_lock(&obj->mutStreamFlag);
			obj->notification_flag = - obj->notification_flag;
			obj->database.push_data("notification_flag", obj->notification_flag);
			obj->database.upload_video(video_file);
			
			pthread_mutex_unlock(&obj->mutStreamFlag);
		
		}
		pthread_mutex_unlock(&obj->mutCamFrame);
		
	}
	return NULL;
}

void* CLocalS :: tLivestreamAudio(void *arg){

	CLocalS *obj = reinterpret_cast<CLocalS*>(arg);

	while(obj){
		
		pthread_mutex_lock(&obj->mutStreamFlag);
		
		pthread_cond_wait(&obj->condAudioFlag, &obj->mutStreamFlag);

		obj->livestream.play_audio();
  
  		obj->database.push_data("new_audio", 0);
  
		pthread_mutex_unlock(&obj->mutStreamFlag);
	}

	return NULL;
}

void CLocalS::sigHandler(int sig)
{
	switch(sig)
	{
		case SIGINT:
			exit(0);
		break;
		default:
			exit(1);
	}
}

