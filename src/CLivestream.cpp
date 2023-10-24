#include "../inc/CLivestream.h"
#include <unistd.h>
#include <sys/wait.h>


CLivestream :: CLivestream() {
        //system("sudo systemctl start nginx.service");
}

CLivestream :: ~CLivestream() {
        //system("sudo systemctl stop nginx.service");
}

void CLivestream :: start() {
        /*system("ffmpeg -re \
        -f v4l2 -video_size 320x240 -thread_queue_size 16384 -framerate 70 -i /dev/video0 \
        -f alsa -ac 1 -thread_queue_size 1024 -ar 44100 -i plughw:0,0 \
        -f flv rtmp:192.168.109.84/live\
        -c:a libfdk_aac -b:a 128k -c:v libx264 -b:v 1600k -qp 0  -preset ultrafast -filter:v fps=fps=30 -tune zerolatency -x264opts keyint=50 -g 25 -pix_fmt yuv420p ");
		*/
		
	pid_t pid = fork();
    if (pid == 0) {
        // Child process
        char *args[] = {"ffmpeg", "-re", "-f", "v4l2", "-video_size", "320x240", "-thread_queue_size", "16384", "-framerate", "70", "-i", "/dev/video0", "-f", "alsa", "-ac", "1", "-thread_queue_size", "1024", "-ar", "44100", "-i", "hw:1", "-f", "flv", "rtmp://localhost/live", "-c:a", "libfdk_aac", "-b:a", "128k", "-c:v", "libx264", "-b:v", "1600k", "-qp", "0", "-preset", "ultrafast", "-filter:v", "fps=fps=30", "-tune", "zerolatency", "-x264opts", "keyint=50", "-g", "25", "-pix_fmt", "yuv420p", NULL};
        execvp("ffmpeg", args);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, WNOHANG);
    }	
		
		
        /*      1 line : instructs FFmpeg to read the same number of frames per second as the framerate of the input video
                2 line : grab video from V4L2 video source; set the frame size; sets the maximum number of queued packets when reading from device; sets the number of frames per second; 
                3 line : grab audio from ALSA audio source; set the number of audio channels as 1; sets the maximum number of queued packets when reading from device ; converts a.wav to MPEG audio at 44100 Hz sample rate; choose the input audio device
                4 line : copy audio streams and encde video streams with h264; set the video bitrate of output file to 2048K/bits; econding speed to compression ratio, higher means lower quality; filter the video stream using the filtergraph specified by filtergraph; set the video fps; zerolatency good for fast encoding and low-latency streaming
                5 line : defines the output container format of the videon in the rtmp case is flv (flashvideo); target RTMP destination for the video. The url is based on the configuration of the streaming server.
        */

}



void CLivestream :: stop() {
        
        system("pidof ffmpeg | xargs kill -9");
	
}

void CLivestream :: play_audio(){
 
 	//system("omxplayer https://firebasestorage.googleapis.com/v0/b/testapp-e0800.appspot.com/o/audio_livestream%2Fmp3_file.mp3?alt=media");
  
 
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        char *args[] = {"omxplayer", "--vol" , "2200" , "https://firebasestorage.googleapis.com/v0/b/testapp-e0800.appspot.com/o/audio_livestream%2Fmp3_file.mp3?alt=media", NULL};
        execvp("omxplayer", args);    
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, WNOHANG);
    }
}
