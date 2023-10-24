#ifndef __CCHARDEVDRIVER_H__
#define __CCHARDEVDRIVER_H__

#include <string>

class CCharDevDriver
{
public:
	CCharDevDriver(std::string devName);
	~CCharDevDriver();
    
	void Open();
	void Close();

    virtual void enable(void);
	virtual void disable(void);

protected:
	int dev;

private:

	std::string devStr;
};

#endif
