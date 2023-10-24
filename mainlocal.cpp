#define DEBUG

#include "debug.h"
#include "CLocalS.h"
#include <iostream>


using namespace std;


int main(int argc, char *argv[])
{
	CLocalS localSys;
	DEBUG_MSG("Processo principal começa");

	localSys.run();

	cout << endl << "LocalSys: Exited main()" << endl;
	return 0;
}
