#include "CDatabase.h"
#include <iostream>
#include <utils.h>
#include <string>

using namespace std;

CDatabase::CDatabase(std::string pID)
{
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");
    productID = pID;

    pName = PyUnicode_FromString((char*)"database");

    //Import the Python Script
    pModule = PyImport_Import(pName);

    if(pModule == NULL)
    {
        panic("No module with that name");
        exit(1);
    }

    //Get the dictionary referent to the imported module
    pDict = PyModule_GetDict(pModule);

    Py_DECREF(pName);
    Py_DECREF(pModule);

    pFuncInit_database = PyDict_GetItemString(pDict, (char*)"init_database");
    
    //Check if its callable
    if (!PyCallable_Check(pFuncInit_database))
    {
        PyErr_Print();
        exit(1);
    }

    pFuncGet_data = PyDict_GetItemString(pDict, (char*)"get_data");
    
    //Check if its callable
    if (!PyCallable_Check(pFuncGet_data))
    {
        PyErr_Print();
        exit(1);
    }

    pFuncPush_data = PyDict_GetItemString(pDict, (char*)"push_data");
    
    //Check if its callable
    if (!PyCallable_Check(pFuncPush_data))
    {
        PyErr_Print();
        exit(1);
    }

    pFuncUpload_video = PyDict_GetItemString(pDict, (char*)"upload_video");
    
    //Check if its callable
    if (!PyCallable_Check(pFuncUpload_video))
    {
        PyErr_Print();
        exit(1);
    }

    init_database();
}

CDatabase::~CDatabase(){
    Py_Finalize();
}

int CDatabase::init_database()
{
    pResult = PyObject_CallFunction(pFuncInit_database,"s",productID.c_str());
    if(pResult == NULL)
    {
        PyErr_Print();
        exit(1);
    }

    //Cleanup
    Py_XDECREF(pResult);

    return 1;
}

int CDatabase::get_data(std::string data_child)
{
    pResult = PyObject_CallFunction(pFuncGet_data,"ss",productID.c_str(),data_child.c_str());
    if(pResult == NULL)
    {
        PyErr_Print();
        return 0;
    }

    //Truncates the result to int since the Python function results
    //Translates from Python to C double
    int aux = (int)PyLong_AsLong(pResult);

    if (aux < 0)
    {
        //Cleanup
        Py_XDECREF(pResult);
        panic("ERROR GETTING FLAG");
        return -7;
    }

    //Cleanup
    Py_XDECREF(pResult);

    return aux;
}

int CDatabase::push_data(std::string data_child, int data_value)
{
    //Call the function with arguments -> i = one integer , "ii" = two integers , "d" -> double mkvalue-style format string
    pResult = PyObject_CallFunction(pFuncPush_data,"ssi",productID.c_str(),data_child.c_str(),data_value);
    if(pResult == NULL)
    {
        PyErr_Print();
        return 0;
    }
    //Cleanup
    Py_XDECREF(pResult);

    return 1;
}

int CDatabase::upload_video(std::string video_file)
{
    //Call the function with arguments -> i = one integer , "ii" = two integers , "d" -> double mkvalue-style format string
    pResult = PyObject_CallFunction(pFuncUpload_video,"ss",productID.c_str(),video_file.c_str());

    if(pResult == NULL)
    {
        PyErr_Print();
        return 0;
    }
    //Cleanup
    Py_XDECREF(pResult);


    return 1;
}
