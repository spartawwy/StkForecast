#if 0

#include "stdafx.h"
#include <string>
#include <iostream>

#include "Python.h"
#include "frameobject.h"

#include <list> 

#include <algorithm>
#include <stdio.h>

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
      
    // 将Python工作路径切换到待调用模块所在目录，一定要保证路径名的正确性
    string path = "D:\\ProgramFilesBase\\Python\\Python36-32\\Lib\\site-packages\\tushare";
    string chdir_cmd = string("sys.path.append(\"") + path + "\")";
    const char* cstr_cmd = chdir_cmd.c_str();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString(cstr_cmd);

    //导入模块  
    PyObject* pModule = PyImport_ImportModule("tushare"); 
    if (!pModule)  
    {  
        cout<<"Python get tushare module failed." << endl;  
        return 0;  
    }  
    auto pDict1 = PyModule_GetDict(pModule);
    if ( !pDict1 ) 
    {          
        return -1;    
    }    
     
    PyObject* pv = PyObject_GetAttrString(pModule, "get_k_data");  
    if (!pv || !PyCallable_Check(pv))  
    {  
        cout << "Can't find funftion (_add)" << endl;  
        return 0;  
    }  
    cout << "Get function (_add) succeed." << endl;  

    PyObject *args = PyTuple_New(1);
    PyObject *arg0 = Py_BuildValue("s", "002337");
    PyTuple_SetItem(args, 0, arg0);
    PyObject* pRet = PyEval_CallObject(pv, args);
     
    if (pRet)  
    {   
        int list_len = PyObject_Size(pRet); 
        PyObject *list_item = NULL;    
        for (int i = 0; i < list_len; i++)
        {
            list_item = PyList_GetItem(pRet, i); //ps: list_item ret null, why ?

        }  
        PyObject *list1;
#if 0
        PyArg_Parse(pRet, "O", &list1);
        // PyList_CheckExact
        if(PyList_Check(list1))  // crash
        {
            printf( "yes it's list\n" );
        }
#endif
       
    }  
 
     return 0;
}

#endif