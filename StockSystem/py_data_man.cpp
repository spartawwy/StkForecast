#ifdef USE_PYTHON_QUOTE

#include "py_data_man.h"

#include <string>
#include <iostream>
#include <list>
#include <vector> 
#include <algorithm>

#include "Python.h"
#include "frameobject.h"

#ifdef _DEBUG
#undef Py_XDECREF
#define Py_XDECREF(a) (a)
#endif

PyDataMan::PyDataMan() : getDayKline_func_(nullptr), getRealTimeK_func_(nullptr)
{

}

PyDataMan::~PyDataMan()
{
    Py_Finalize();
}

bool PyDataMan::Initiate()
{
    int r = Py_IsInitialized();  //1为已经初始化了
    if (r != 0)
    {
        std::cout << "init python fail " << std::endl;
        return false;
    }
      
    Py_Initialize(); //初始化
    auto p_main_Module = PyImport_ImportModule("__main__");
    if (!p_main_Module)
    {
        throw "";
    }
    auto pDict = PyModule_GetDict(p_main_Module);
    if ( !pDict ) 
    {          
        return false;    
    }     

    PyRun_SimpleString("import sys");
  
	char stk_py_dir[256] = {0};
	unsigned int ret_env_size = sizeof(stk_py_dir);
	getenv_s(&ret_env_size, stk_py_dir, ret_env_size, "STK_PY_DIR");
    std::string path = stk_py_dir;
    std::string chdir_cmd = std::string("sys.path.append(\"") + path + "\")";
    const char* cstr_cmd = chdir_cmd.c_str(); 
    PyRun_SimpleString(cstr_cmd);

    PyObject* pModule = PyImport_ImportModule("get_stk_kline"); // get_stock_his.py
    if (!pModule)  
    {  
        std::cout<<"Python get get_stk_his_fill2file module failed." << std::endl;  
        return false;  
    }  
    PyObject *pDic = PyModule_GetDict(pModule);
    if (!pDic)  
    {  
        std::cout<<"Python get pDic fail" << std::endl;  
        return false;  
    }  

    PyObject* pcls = PyObject_GetAttrString(pModule, "KLINE");   
    if (!pcls || !PyCallable_Check(pcls))  
    {  
        std::cout << "Can't get class KLINE" << std::endl;  
        return false;  
    }  
 
    PyObject* p_stock_obj = PyEval_CallObject(pcls, NULL);
    if( !p_stock_obj )
    {
        std::cout << "Can't create KLINE obj" << std::endl;  
        return false;  
    }
    getDayKline_func_ = PyObject_GetAttrString(p_stock_obj, "getDayKline");
    
#if 0
    auto pArg = PyTuple_New(3);
    PyTuple_SetItem(pArg, 0, Py_BuildValue("s", "601699"));
    PyTuple_SetItem(pArg, 1, Py_BuildValue("s", "2017-12-07"));
    PyTuple_SetItem(pArg, 2, Py_BuildValue("s", "2018-03-08"));
    auto pRet = PyEval_CallObject(getDayKline_func, pArg);
    char *result;
    PyArg_Parse(pRet, "s", &result);
    Py_XDECREF(result);
    return getDayKline_func_;
#endif
    getRealTimeK_func_ = PyObject_GetAttrString(pModule, "get_realtime_k_data"); 
     
    return true;
}

#endif // #ifdef USE_PYTHON_QUOTE