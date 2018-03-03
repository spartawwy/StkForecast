#include "stdafx.h"
#include <string>
#include <iostream>

#include "Python.h"
#include "frameobject.h"

#include <list>
#include <vector>

#include <algorithm>
#include <stdio.h>

#ifdef TO_GET_DATA

#pragma comment(lib, "python36.lib")

using namespace std;

int main()
{ 
    int r = Py_IsInitialized();  //1为已经初始化了
    if (r != 0)
    {
        cout << "init python fail " << endl;
        return 0;
    }
     
    //Py_SetPythonHome(L"C:\\Python35");
    Py_Initialize(); //初始化
    auto p_main_Module = PyImport_ImportModule("__main__");
    if (!p_main_Module)
    {
        throw "";
    }
    auto pDict = PyModule_GetDict(p_main_Module);
    if ( !pDict ) 
    {          
        return -1;    
    }     

    PyRun_SimpleString("import sys");

    string path = "D:\\ProgramFilesBase\\Python";
    string chdir_cmd = string("sys.path.append(\"") + path + "\")";
    const char* cstr_cmd = chdir_cmd.c_str(); 
    PyRun_SimpleString(cstr_cmd);

    //导入模块  
    //PyObject* pModule = PyImport_ImportModule("test"); 
    PyObject* pModule = PyImport_ImportModule("get_stock_his"); // get_stock_his.py
    if (!pModule)  
    {  
        cout<<"Python get tushare module failed." << endl;  
        return 0;  
    }  
     
    PyObject* pv = PyObject_GetAttrString(pModule, "get_stock_hisdata");   
    if (!pv || !PyCallable_Check(pv))  
    {  
        cout << "Can't find funftion (_add)" << endl;  
        return 0;  
    }  
      
    //PyObject* pobj1 = Py_BuildValue("(s)", "hello"); '600104', '2018-02-13')   
     PyObject *args = PyTuple_New(2);
     PyObject *arg0 = Py_BuildValue("s", "600104");
     PyObject *arg1 = Py_BuildValue("s", "2018-02-13");
     PyTuple_SetItem(args, 0, arg0);
     PyTuple_SetItem(args, 1, arg1);
     PyObject* pRet = PyEval_CallObject(pv, args);  
     auto val = Py_TYPE(pRet);
    char *result;
    PyArg_Parse(pRet, "s", &result);
    printf("PyEval_CallObject get %s", result);

    return 0;
}

#endif // TO_GET_DATA