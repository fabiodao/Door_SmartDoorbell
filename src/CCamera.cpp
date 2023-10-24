
#include <iostream>
#include "CCamera.h"
#include </usr/local/include/opencv4/opencv2/opencv.hpp>

#include <utils.h>





/*
#define FRAME_W 		640
#define FRAME_H 		480

#define CAM_BRIGHTNESS	0.4  //-> 20
#define CAM_CONTRAST	0.50 //-> 0
#define CAM_SATURATION	0.6  //-> 10
*/

#define deviceID 0

using namespace cv;
using namespace std;



CCamera::CCamera()
{

}

CCamera::~CCamera(){

}

string CCamera::record(){

   dt = ctime(&now);
   now = time(0);
   delay = 5;
   
   
   delay*= CLOCKS_PER_SEC;

   video_file.clear();

   video_file = ".mp4";
   video_file.insert(0,dt);

   video_file[dt.length()-1]=' ';	
	
   replace(video_file.begin(), video_file.end(), ' ', '_');
   if(video_file[8]=='_')
	video_file[8]='0';
   
   open();
   
   if(!Camera.isOpened()){
      panic ("Error! Enable to open camera\n");
    }
    
    frame_width = Camera.get(cv::CAP_PROP_FRAME_WIDTH);
    frame_height = Camera.get(cv::CAP_PROP_FRAME_HEIGHT);	 

   VideoWriter video(video_file, cv::VideoWriter::fourcc('m','p','4','v'), 20, Size(frame_width,frame_height));

   if (!Camera.isOpened()) {
        panic ("Error! Could not open the output video file for write\n");
    }

   time_now = clock();
  
   while(clock() - time_now < delay){

      Camera >> frame;		

      if (frame.empty())
        panic("Error! Blank frame captured.\n");

      video.write(frame);
   }

   close();

   return video_file;

}

bool CCamera::detectface(){
 
   open();

   if(!Camera.isOpened()){
      panic ("Error! Enable to open camera\n");
   }

   Camera.read(frame);

   	
   CascadeClassifier facedetect;
   facedetect.load("/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml");

   facedetect.detectMultiScale(frame,faces,1.3,5);
   
   close();
		
   if (faces.size()!=0){
   	return true;
   }
   else 
   	return false;

}


void CCamera::open(){
   
   Camera.open(deviceID, cv::CAP_V4L2);

}

void CCamera::close(){

   Camera.release();
}
