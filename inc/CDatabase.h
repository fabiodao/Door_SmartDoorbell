#ifndef __CDATABASE_H__
#define __CDATABASE_H__

#include </home/vitor/buildroots/Fabio/buildroot-2021.08/output/host/include/python3.9/Python.h>
#include <string>
#include <iostream>
#include <stdlib.h>


class CDatabase{
    private:
        PyObject *pName, *pModule, *pDict;
        PyObject *pFuncInit_database,*pFuncGet_data,*pFuncPush_data,*pFuncUpload_video,*pResult;
        std::string productID;
    
    public:
        CDatabase(std::string pID);
        ~CDatabase();
        
        int init_database();
        int get_data(std::string data_child);
        int push_data(std::string data_child, int value);
        int upload_video(std::string video_file);

};

#endif
