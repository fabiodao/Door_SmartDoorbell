#ifndef CCamera_h
#define CCamera_h

#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include </usr/local/include/opencv4/opencv2/opencv.hpp>
#include <unistd.h>
#include <ctime>
#include <string>
#include<cstdio>
#include <algorithm>

using namespace cv;
using namespace std;


class CCamera{

private:

    cv::Mat frame;
    cv::VideoCapture Camera;
    int frame_width;
    int frame_height;	

    string video_file;
    
    string dt;
    time_t now;
    
    int delay;	
    clock_t time_now;
    
    vector<Rect> faces;
public:

    CCamera();
    ~CCamera();
    void capture();
    string record();
    bool detectface();
    void open();
    void close();

};

#endif
