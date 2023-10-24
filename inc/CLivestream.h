#ifndef CLivestream_h
#define CLivestream_h

#include <stdio.h>
#include <iostream>
#include <pthread.h>
using namespace std;


class CLivestream{

private:

public:
    CLivestream();
    ~CLivestream();

    void start();
    void stop();
    void play_audio();
};

#endif
