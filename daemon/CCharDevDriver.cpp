#include "CCharDevDriver.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define IOCTL_PID 1

CCharDevDriver::CCharDevDriver(string devName)
{
	this->devStr = "/dev/" + devName;
}

CCharDevDriver::~CCharDevDriver()
{
	close(dev);
}

void CCharDevDriver::Open()
{
	pid_t pid;
	
	this->dev = open(devStr.c_str(), O_RDWR);

	if(dev < 0)
	{	
		std::cout<<"Device driver not found";
	}
	
	pid = getpid();
	if(ioctl(dev, IOCTL_PID, &pid))
	{
		close(dev);
		std::cout<<"Failed system call. Closing device driver.";
	}
}

void CCharDevDriver::enable(void){}
void CCharDevDriver::disable(void){}

void CCharDevDriver::Close()
{
	close(dev);
}
